// Copyright (c) 2019 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_NET_PROXYING_URL_LOADER_FACTORY_H_
#define SHELL_BROWSER_NET_PROXYING_URL_LOADER_FACTORY_H_

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "base/optional.h"
#include "services/network/public/cpp/resource_request.h"
#include "services/network/public/cpp/resource_response.h"
#include "services/network/public/mojom/network_context.mojom.h"
#include "services/network/public/mojom/url_loader.mojom.h"
#include "shell/browser/net/atom_url_loader_factory.h"

namespace electron {

// This class is responsible for following tasks when NetworkService is enabled:
// 1. handling intercepted protocols;
// 2. implementing webRequest module;
//
// For the task #2, the code is referenced from the
// extensions::WebRequestProxyingURLLoaderFactory class.
class ProxyingURLLoaderFactory
    : public network::mojom::URLLoaderFactory,
      public network::mojom::TrustedURLLoaderHeaderClient {
 public:
  class InProgressRequest : public network::mojom::URLLoader,
                            public network::mojom::URLLoaderClient,
                            public network::mojom::TrustedHeaderClient {
   public:
    InProgressRequest(
        ProxyingURLLoaderFactory* factory,
        int32_t routing_id,
        int32_t network_service_request_id,
        uint32_t options,
        const network::ResourceRequest& request,
        const net::MutableNetworkTrafficAnnotationTag& traffic_annotation,
        network::mojom::URLLoaderRequest loader_request,
        network::mojom::URLLoaderClientPtr client);
    ~InProgressRequest() override;

    void Restart();

    // network::mojom::URLLoader:
    void FollowRedirect(const std::vector<std::string>& removed_headers,
                        const net::HttpRequestHeaders& modified_headers,
                        const base::Optional<GURL>& new_url) override;
    void SetPriority(net::RequestPriority priority,
                     int32_t intra_priority_value) override;
    void PauseReadingBodyFromNet() override;
    void ResumeReadingBodyFromNet() override;

    // network::mojom::URLLoaderClient:
    void OnReceiveResponse(const network::ResourceResponseHead& head) override;
    void OnReceiveRedirect(const net::RedirectInfo& redirect_info,
                           const network::ResourceResponseHead& head) override;
    void OnUploadProgress(int64_t current_position,
                          int64_t total_size,
                          OnUploadProgressCallback callback) override;
    void OnReceiveCachedMetadata(mojo_base::BigBuffer data) override;
    void OnTransferSizeUpdated(int32_t transfer_size_diff) override;
    void OnStartLoadingResponseBody(
        mojo::ScopedDataPipeConsumerHandle body) override;
    void OnComplete(const network::URLLoaderCompletionStatus& status) override;

    void OnLoaderCreated(network::mojom::TrustedHeaderClientRequest request);

    // network::mojom::TrustedHeaderClient:
    void OnBeforeSendHeaders(const net::HttpRequestHeaders& headers,
                             OnBeforeSendHeadersCallback callback) override;
    void OnHeadersReceived(const std::string& headers,
                           OnHeadersReceivedCallback callback) override;

   private:
    // These two methods combined form the implementation of Restart().
    void UpdateRequestInfo();
    void RestartInternal();

    void ContinueToBeforeSendHeaders(int error_code);
    void ContinueToSendHeaders(const std::set<std::string>& removed_headers,
                               const std::set<std::string>& set_headers,
                               int error_code);
    void ContinueToStartRequest(int error_code);
    void ContinueToHandleOverrideHeaders(int error_code);
    void ContinueToResponseStarted(int error_code);
    void ContinueToBeforeRedirect(const net::RedirectInfo& redirect_info,
                                  int error_code);
    void HandleBeforeRequestRedirect();
    void HandleResponseOrRedirectHeaders(
        net::CompletionOnceCallback continuation);
    void OnRequestError(const network::URLLoaderCompletionStatus& status);

    ProxyingURLLoaderFactory* factory_;
    network::ResourceRequest request_;
    const base::Optional<url::Origin> original_initiator_;
    const int32_t routing_id_;
    const int32_t network_service_request_id_;
    const uint32_t options_;
    const net::MutableNetworkTrafficAnnotationTag traffic_annotation_;
    mojo::Binding<network::mojom::URLLoader> proxied_loader_binding_;
    network::mojom::URLLoaderClientPtr target_client_;

    network::ResourceResponseHead current_response_;
    scoped_refptr<net::HttpResponseHeaders> override_headers_;
    GURL redirect_url_;

    mojo::Binding<network::mojom::URLLoaderClient> proxied_client_binding_;
    network::mojom::URLLoaderPtr target_loader_;

    bool request_completed_ = false;

    // If |has_any_extra_headers_listeners_| is set to true, the request will be
    // sent with the network::mojom::kURLLoadOptionUseHeaderClient option, and
    // we expect events to come through the
    // network::mojom::TrustedURLLoaderHeaderClient binding on the factory. This
    // is only set to true if there is a listener that needs to view or modify
    // headers set in the network process.
    bool has_any_extra_headers_listeners_ = false;
    bool current_request_uses_header_client_ = false;
    OnBeforeSendHeadersCallback on_before_send_headers_callback_;
    OnHeadersReceivedCallback on_headers_received_callback_;
    mojo::Binding<network::mojom::TrustedHeaderClient> header_client_binding_;

    // If |has_any_extra_headers_listeners_| is set to false and a redirect is
    // in progress, this stores the parameters to FollowRedirect that came from
    // the client. That way we can combine it with any other changes that
    // extensions made to headers in their callbacks.
    struct FollowRedirectParams {
      FollowRedirectParams();
      ~FollowRedirectParams();
      std::vector<std::string> removed_headers;
      net::HttpRequestHeaders modified_headers;
      base::Optional<GURL> new_url;

      DISALLOW_COPY_AND_ASSIGN(FollowRedirectParams);
    };
    std::unique_ptr<FollowRedirectParams> pending_follow_redirect_params_;

    base::WeakPtrFactory<InProgressRequest> weak_factory_{this};

    DISALLOW_COPY_AND_ASSIGN(InProgressRequest);
  };

  ProxyingURLLoaderFactory(
      const HandlersMap& intercepted_handlers,
      network::mojom::URLLoaderFactoryRequest loader_request,
      network::mojom::URLLoaderFactoryPtrInfo target_factory_info,
      network::mojom::TrustedURLLoaderHeaderClientRequest
          header_client_request);
  ~ProxyingURLLoaderFactory() override;

  // network::mojom::URLLoaderFactory:
  void CreateLoaderAndStart(network::mojom::URLLoaderRequest loader,
                            int32_t routing_id,
                            int32_t request_id,
                            uint32_t options,
                            const network::ResourceRequest& request,
                            network::mojom::URLLoaderClientPtr client,
                            const net::MutableNetworkTrafficAnnotationTag&
                                traffic_annotation) override;
  void Clone(network::mojom::URLLoaderFactoryRequest request) override;

  // network::mojom::TrustedURLLoaderHeaderClient:
  void OnLoaderCreated(
      int32_t request_id,
      network::mojom::TrustedHeaderClientRequest request) override;

 private:
  void OnTargetFactoryError();
  void OnProxyBindingError();

  // This is passed from api::ProtocolNS.
  //
  // The ProtocolNS instance lives through the lifetime of BrowserContenxt,
  // which is guarenteed to cover the lifetime of URLLoaderFactory, so the
  // reference is guarenteed to be valid.
  //
  // In this way we can avoid using code from api namespace in this file.
  const HandlersMap& intercepted_handlers_;

  mojo::BindingSet<network::mojom::URLLoaderFactory> proxy_bindings_;
  network::mojom::URLLoaderFactoryPtr target_factory_;
  mojo::Binding<network::mojom::TrustedURLLoaderHeaderClient>
      url_loader_header_client_binding_;

  DISALLOW_COPY_AND_ASSIGN(ProxyingURLLoaderFactory);
};

}  // namespace electron

#endif  // SHELL_BROWSER_NET_PROXYING_URL_LOADER_FACTORY_H_
