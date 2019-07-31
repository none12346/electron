// Copyright (c) 2014 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_API_ATOM_API_TRAY_H_
#define SHELL_BROWSER_API_ATOM_API_TRAY_H_

#include <memory>
#include <string>
#include <vector>

#include "gin/arguments.h"
#include "gin/dictionary.h"
#include "gin/handle.h"
#include "gin/object_template_builder.h"
#include "gin/wrappable.h"
// #include "shell/browser/api/trackable_object.h"
#include "shell/browser/ui/tray_icon.h"
#include "shell/browser/ui/tray_icon_observer.h"

namespace gfx {
class Image;
}

namespace electron {

class TrayIcon;

namespace api {

class Menu;
class NativeImage;

class Tray : public gin::Wrappable<Tray>, public TrayIconObserver {
 public:
  static gin::Handle<Tray> Create(v8::Isolate* isolate, gin::Arguments* args);

  // gin::Wrappable
  gin::ObjectTemplateBuilder GetObjectTemplateBuilder(
      v8::Isolate* isolate) override;

  static gin::WrapperInfo kWrapperInfo;

 protected:
  Tray();
  ~Tray() override;

  // TrayIconObserver:
  void OnClicked(const gfx::Rect& bounds,
                 const gfx::Point& location,
                 int modifiers) override;
  void OnDoubleClicked(const gfx::Rect& bounds, int modifiers) override;
  void OnRightClicked(const gfx::Rect& bounds, int modifiers) override;
  void OnBalloonShow() override;
  void OnBalloonClicked() override;
  void OnBalloonClosed() override;
  void OnDrop() override;
  void OnDropFiles(const std::vector<std::string>& files) override;
  void OnDropText(const std::string& text) override;
  void OnDragEntered() override;
  void OnDragExited() override;
  void OnDragEnded() override;
  void OnMouseEntered(const gfx::Point& location, int modifiers) override;
  void OnMouseExited(const gfx::Point& location, int modifiers) override;
  void OnMouseMoved(const gfx::Point& location, int modifiers) override;

  void SetImage(v8::Isolate* isolate, gin::Handle<NativeImage> image);
  //   void SetPressedImage(v8::Isolate* isolate, gin::Handle<NativeImage>
  //   image);
  void SetToolTip(const std::string& tool_tip);
  void SetTitle(const std::string& title);
  std::string GetTitle();
  void SetHighlightMode(TrayIcon::HighlightMode mode);
  void SetIgnoreDoubleClickEvents(bool ignore);
  bool GetIgnoreDoubleClickEvents();
  //   void DisplayBalloon(gin::Arguments* args, gin::Dictionary& options);
  //   void PopUpContextMenu(gin::Arguments* args);
  //   void SetContextMenu(v8::Isolate* isolate, gin::Handle<Menu> menu);
  //   gfx::Rect GetBounds();

 private:
  v8::Global<v8::Object> menu_;
  std::unique_ptr<TrayIcon> tray_icon_;

  DISALLOW_COPY_AND_ASSIGN(Tray);
};

}  // namespace api

}  // namespace electron

#endif  // SHELL_BROWSER_API_ATOM_API_TRAY_H_
