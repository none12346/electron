// Copyright (c) 2019 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/api/atom_api_demo.h"

#include "gin/dictionary.h"
#include "gin/function_template.h"
#include "shell/common/node_includes.h"

namespace electron {

namespace api {

gin::WrapperInfo Demo::kWrapperInfo = {gin::kEmbedderNativeGin};

Demo::Demo() {
  LOG(INFO) << "Demo()";
}

Demo::~Demo() {
  LOG(INFO) << "~Demo()";
}

// static
// mate::WrappableBase* Tray::New(mate::Handle<NativeImage> image,
//                                mate::Arguments* args) {
//   if (!Browser::Get()->is_ready()) {
//     args->ThrowError("Cannot create Tray before app is ready");
//     return nullptr;
//   }
//   return new Tray(args->isolate(), args->GetThis(), image);
// }

// static
// void Tray::BuildPrototype(v8::Isolate* isolate,
//                           v8::Local<v8::FunctionTemplate> prototype) {
//   prototype->SetClassName(mate::StringToV8(isolate, "Tray"));
//   mate::ObjectTemplateBuilder(isolate, prototype->PrototypeTemplate())
//       .MakeDestroyable()
//       .SetMethod("setImage", &Tray::SetImage)
//       .SetMethod("setPressedImage", &Tray::SetPressedImage)
//       .SetMethod("setToolTip", &Tray::SetToolTip)
//       .SetMethod("setTitle", &Tray::SetTitle)
//       .SetMethod("getTitle", &Tray::GetTitle)
//       .SetMethod("setHighlightMode", &Tray::SetHighlightMode)
//       .SetMethod("setIgnoreDoubleClickEvents",
//                  &Tray::SetIgnoreDoubleClickEvents)
//       .SetMethod("getIgnoreDoubleClickEvents",
//                  &Tray::GetIgnoreDoubleClickEvents)
//       .SetMethod("displayBalloon", &Tray::DisplayBalloon)
//       .SetMethod("popUpContextMenu", &Tray::PopUpContextMenu)
//       .SetMethod("setContextMenu", &Tray::SetContextMenu)
//   .SetMethod("getBounds", &Tray::GetBounds);
// }

// static
gin::Handle<Demo> Demo::Create(v8::Isolate* isolate) {
  return gin::CreateHandle(isolate, new Demo());
}

gin::ObjectTemplateBuilder Demo::GetObjectTemplateBuilder(
    v8::Isolate* isolate) {
  return gin::Wrappable<Demo>::GetObjectTemplateBuilder(isolate).SetValue("abc",
                                                                          12);
}

}  // namespace api

}  // namespace electron

namespace {

void Initialize(v8::Local<v8::Object> exports,
                v8::Local<v8::Value> unused,
                v8::Local<v8::Context> context,
                void* priv) {
  v8::Isolate* isolate = context->GetIsolate();
  gin::Dictionary dict(isolate, exports);

  auto constructor =
      gin::CreateFunctionTemplate(
          isolate, base::BindRepeating(&electron::api::Demo::Create), {true})
          ->GetFunction(isolate->GetCurrentContext())
          .ToLocalChecked();

  dict.Set("constructor", constructor);

  //   Tray::SetConstructor(isolate, base::BindRepeating(&Tray::New));

  //   mate::Dictionary dict(isolate, exports);
  //   dict.Set(
  //       "Tray",
  //       Tray::GetConstructor(isolate)->GetFunction(context).ToLocalChecked());
}

}  // namespace

NODE_LINKED_MODULE_CONTEXT_AWARE(atom_browser_demo, Initialize)