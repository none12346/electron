// Copyright (c) 2019 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_API_ATOM_API_DEMO_H_
#define SHELL_BROWSER_API_ATOM_API_DEMO_H_

#include "gin/handle.h"
#include "gin/object_template_builder.h"
#include "gin/wrappable.h"

namespace electron {

namespace api {

class Demo : public gin::Wrappable<Demo> {
 public:
  static gin::Handle<Demo> Create(v8::Isolate* isolate);

  // gin::Wrappable
  gin::ObjectTemplateBuilder GetObjectTemplateBuilder(
      v8::Isolate* isolate) override;

  static gin::WrapperInfo kWrapperInfo;

 protected:
  explicit Demo();
  ~Demo() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(Demo);
};

}  // namespace api

}  // namespace electron

#endif  // SHELL_BROWSER_API_ATOM_API_DEMO_H_
