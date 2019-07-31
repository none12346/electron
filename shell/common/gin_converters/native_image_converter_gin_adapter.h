// Copyright (c) 2019 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_COMMON_GIN_CONVERTERS_GFX_CONVERTER_GIN_ADAPTER_H_
#define SHELL_COMMON_GIN_CONVERTERS_GFX_CONVERTER_GIN_ADAPTER_H_

#include "gin/converter.h"
#include "gin/wrappable.h"
// #include "shell/common/api/atom_api_native_image.h"

// TODO(deermichel): replace adapter with real converter after removing mate
// -- this adapter forwards all conversions to the existing mate converter --
// (other direction might be preferred, but this is safer for now :D)

namespace electron {
namespace api {
class NativeImage;
}
}  // namespace electron

namespace gin {

template <>
struct Converter<gin::Handle<electron::api::NativeImage>> {
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     gin::Handle<electron::api::NativeImage>* out) {
    // WrappableBase* wrapper =
    // static_cast<WrappableBase*>(internal::FromV8Impl(isolate, val, {})); if
    // (!wrapper)
    //     return false;

    // *out = CreateHandle(isolate,
    // static_cast<electron::api::NativeImage*>(wrapper));
    return false;

    // static mate::Handle<electron::api::NativeImage>* mateOut = nullptr;

    // if (mate::ConvertFromV8(isolate, val, mateOut)) {
    //     *out = gin::CreateHandle(isolate, mateOut->get());
    //     return true;
    // }

    //     WrappableBase* wrapper =
    //       static_cast<WrappableBase*>(internal::FromV8Impl(isolate, val));
    //   if (!wrapper)
    //     return false;

    //   *out = CreateHandle(isolate,
    //   static_cast<electron::api::NativeImage*>(wrapper)); return true;
  }
};

}  // namespace gin

#endif  // SHELL_COMMON_GIN_CONVERTERS_GFX_CONVERTER_GIN_ADAPTER_H_
