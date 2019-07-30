// Copyright (c) 2014 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/api/atom_api_tray.h"

#include <string>

#include "base/threading/thread_task_runner_handle.h"
#include "shell/browser/api/atom_api_menu.h"
#include "shell/browser/browser.h"
#include "shell/common/api/atom_api_native_image.h"
#include "shell/common/api/gin_utils.h"
#include "shell/common/gin_converters/gfx_converter_gin_adapter.h"
// #include "shell/common/native_mate_converters/image_converter.h"
// #include "shell/common/native_mate_converters/string16_converter.h"
#include "shell/common/node_includes.h"
#include "ui/gfx/image/image.h"

namespace gin {

template <>
struct Converter<electron::TrayIcon::HighlightMode> {
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     electron::TrayIcon::HighlightMode* out) {
    using HighlightMode = electron::TrayIcon::HighlightMode;
    std::string mode;
    if (ConvertFromV8(isolate, val, &mode)) {
      if (mode == "always") {
        *out = HighlightMode::ALWAYS;
        return true;
      }
      if (mode == "selection") {
        *out = HighlightMode::SELECTION;
        return true;
      }
      if (mode == "never") {
        *out = HighlightMode::NEVER;
        return true;
      }
    }
    return false;
  }
};
}  // namespace gin

namespace electron {

namespace api {

gin::WrapperInfo Tray::kWrapperInfo = {gin::kEmbedderNativeGin};

Tray::Tray() : tray_icon_(TrayIcon::Create()) {
  LOG(INFO) << "Tray()";
  // SetImage(isolate, image);
  tray_icon_->AddObserver(this);
}

Tray::~Tray() {
  LOG(INFO) << "~Tray()";
}

void Tray::OnClicked(const gfx::Rect& bounds,
                     const gfx::Point& location,
                     int modifiers) {
  // EmitWithFlags("click", modifiers, bounds, location);
}

void Tray::OnDoubleClicked(const gfx::Rect& bounds, int modifiers) {
  // EmitWithFlags("double-click", modifiers, bounds);
}

void Tray::OnRightClicked(const gfx::Rect& bounds, int modifiers) {
  // EmitWithFlags("right-click", modifiers, bounds);
}

void Tray::OnBalloonShow() {
  // Emit("balloon-show");
}

void Tray::OnBalloonClicked() {
  // Emit("balloon-click");
}

void Tray::OnBalloonClosed() {
  // Emit("balloon-closed");
}

void Tray::OnDrop() {
  // Emit("drop");
}

void Tray::OnDropFiles(const std::vector<std::string>& files) {
  // Emit("drop-files", files);
}

void Tray::OnDropText(const std::string& text) {
  // Emit("drop-text", text);
}

void Tray::OnMouseEntered(const gfx::Point& location, int modifiers) {
  // EmitWithFlags("mouse-enter", modifiers, location);
}

void Tray::OnMouseExited(const gfx::Point& location, int modifiers) {
  // EmitWithFlags("mouse-leave", modifiers, location);
}

void Tray::OnMouseMoved(const gfx::Point& location, int modifiers) {
  // EmitWithFlags("mouse-move", modifiers, location);
}

void Tray::OnDragEntered() {
  // Emit("drag-enter");
}

void Tray::OnDragExited() {
  // Emit("drag-leave");
}

void Tray::OnDragEnded() {
  // Emit("drag-end");
}

// void Tray::SetImage(v8::Isolate* isolate, gin::Handle<NativeImage> image) {
// #if defined(OS_WIN)
//   tray_icon_->SetImage(image->GetHICON(GetSystemMetrics(SM_CXSMICON)));
// #else
//   tray_icon_->SetImage(image->image());
// #endif
// }

// void Tray::SetPressedImage(v8::Isolate* isolate,
//                            gin::Handle<NativeImage> image) {
// #if defined(OS_WIN)
//   tray_icon_->SetPressedImage(image->GetHICON(GetSystemMetrics(SM_CXSMICON)));
// #else
//   tray_icon_->SetPressedImage(image->image());
// #endif
// }

void Tray::SetToolTip(const std::string& tool_tip) {
  tray_icon_->SetToolTip(tool_tip);
}

void Tray::SetTitle(const std::string& title) {
#if defined(OS_MACOSX)
  tray_icon_->SetTitle(title);
#endif
}

std::string Tray::GetTitle() {
#if defined(OS_MACOSX)
  return tray_icon_->GetTitle();
#else
  return "";
#endif
}

void Tray::SetHighlightMode(TrayIcon::HighlightMode mode) {
  tray_icon_->SetHighlightMode(mode);
}

void Tray::SetIgnoreDoubleClickEvents(bool ignore) {
#if defined(OS_MACOSX)
  tray_icon_->SetIgnoreDoubleClickEvents(ignore);
#endif
}

bool Tray::GetIgnoreDoubleClickEvents() {
#if defined(OS_MACOSX)
  return tray_icon_->GetIgnoreDoubleClickEvents();
#else
  return false;
#endif
}

// void Tray::DisplayBalloon(gin::Arguments* args, gin::Dictionary& options) {
//   gin::Handle<NativeImage> icon;
//   options.Get("icon", &icon);
//   base::string16 title, content;
//   if (!options.Get("title", &title) || !options.Get("content", &content)) {
//     args->ThrowTypeError("'title' and 'content' must be defined");
//     return;
//   }

// #if defined(OS_WIN)
//   tray_icon_->DisplayBalloon(
//       icon.IsEmpty() ? NULL : icon->GetHICON(GetSystemMetrics(SM_CXICON)),
//       title, content);
// #else
//   tray_icon_->DisplayBalloon(icon.IsEmpty() ? gfx::Image() : icon->image(),
//                              title, content);
// #endif
// }

// void Tray::PopUpContextMenu(gin::Arguments* args) {
//   gin::Handle<Menu> menu;
//   args->GetNext(&menu);
//   gfx::Point pos;
//   args->GetNext(&pos);
//   tray_icon_->PopUpContextMenu(pos, menu.IsEmpty() ? nullptr :
//   menu->model());
// }

// void Tray::SetContextMenu(v8::Isolate* isolate, gin::Handle<Menu> menu) {
//   menu_.Reset(isolate, menu.ToV8());
//   tray_icon_->SetContextMenu(menu.IsEmpty() ? nullptr : menu->model());
// }

// gfx::Rect Tray::GetBounds() {
//   return tray_icon_->GetBounds();
// }

// static
gin::Handle<Tray> Tray::Create(v8::Isolate* isolate, gin::Arguments* args) {
  if (!Browser::Get()->is_ready()) {
    args->ThrowTypeError("Cannot create Tray before app is ready");
    // return nullptr; // TODO(deermichel)
  }
  return gin::CreateHandle(isolate, new Tray());
}

gin::ObjectTemplateBuilder Tray::GetObjectTemplateBuilder(
    v8::Isolate* isolate) {
  return gin::Wrappable<Tray>::GetObjectTemplateBuilder(isolate)
      // .MakeDestroyable()
      // .SetMethod("setImage", &Tray::SetImage)
      // .SetMethod("setPressedImage", &Tray::SetPressedImage)
      .SetMethod("setToolTip", &Tray::SetToolTip)
      .SetMethod("setTitle", &Tray::SetTitle)
      .SetMethod("getTitle", &Tray::GetTitle)
      .SetMethod("setHighlightMode", &Tray::SetHighlightMode)
      .SetMethod("setIgnoreDoubleClickEvents",
                 &Tray::SetIgnoreDoubleClickEvents)
      .SetMethod("getIgnoreDoubleClickEvents",
                 &Tray::GetIgnoreDoubleClickEvents);
  // .SetMethod("displayBalloon", &Tray::DisplayBalloon)
  // .SetMethod("popUpContextMenu", &Tray::PopUpContextMenu);
  // .SetMethod("setContextMenu", &Tray::SetContextMenu)
  // .SetMethod("getBounds", &Tray::GetBounds);
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
  dict.Set("createTray",
           gin::ConvertCallbackToV8Leaked(
               isolate, base::BindRepeating(&electron::api::Tray::Create)));
}

}  // namespace

NODE_LINKED_MODULE_CONTEXT_AWARE(atom_browser_tray, Initialize)
