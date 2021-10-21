/**
 * Copyright 2021 Torsten Mehnert
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include <complate/core/basicstream.h>
#include <complate/quickjs/quickjsrendererbuilder.h>
#include <complate/v8/v8platform.h>
#include <complate/v8/v8rendererbuilder.h>

#include <iostream>

using namespace std;
using namespace complate;

static const char VIEWS[] = "function render (v, p, s) { s.writeln(v); }";

int main() {
  Object parameters;
  BasicStream stream(std::cout);

  // clang-format: off
  V8Platform v8Platform;
  V8Renderer v8Renderer = V8RendererBuilder()
      .source(VIEWS)
      .build();
  v8Renderer.render("Works with V8Renderer", parameters, stream);

  QuickJsRenderer quickJsRenderer = QuickJsRendererBuilder()
      .source(VIEWS)
      .build();
  quickJsRenderer.render("Works with QuickJsRenderer", parameters, stream);
  // clang-format: on

  return EXIT_SUCCESS;
}
