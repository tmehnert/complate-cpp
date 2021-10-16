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
#pragma once

#include <complate/core/prototype.h>
#include <complate/core/renderer.h>
#include <complate/core/value.h>

#include <vector>

namespace complate {

/**
 * Renderer which uses the V8 JavaScript Engine to render HTML output.
 */
class V8Renderer : public Renderer {
public:
  /**
   * Constructs a V8Renderer
   *
   * Upon construction an V8 Context will be created an the source
   * bundle will be evaluated. You need to create a single complate::V8Platform
   * first or initialize the V8 JavaScript Engine by yourself.
   *
   * @param source The complate JavaScript source bundle with the views.
   */
  explicit V8Renderer(const std::string &source);

  /**
   * Constructs a V8Renderer
   *
   * Upon construction an V8 Context will be created an the source
   * bundle will be evaluated. You need to create a single complate::V8Platform
   * first or initialize the V8 JavaScript Engine by yourself.
   *
   * @param source The complate JavaScript source bundle with the views.
   * @param prototypes Prototypes for C++ classes to be supported via Proxy.
   * @param bindings Global variables available in every view.
   */
  V8Renderer(const std::string &source,
             const std::vector<Prototype> &prototypes, Object bindings);

  ~V8Renderer() override;

  /**
   * Render a view to a Stream using an Object as parameters.
   *
   * @note This method allows to achieve "progressive rendering".
   *
   * @param view Name of the view you want to be rendered.
   * @param parameters The view Parameters aka 'the Model' which passed to the
   * view.
   * @param stream A stream in which the HTML output will be forwarded.
   */
  void render(const std::string &view, const Object &parameters,
              Stream &stream) override;

  /**
   * Render a view to a Stream using a JSON string as parameters.
   *
   * @note This method allows to achieve "progressive rendering".
   *
   * @param view Name of the view you want to be rendered.
   * @param parameters The view Parameters aka 'the Model' which passed to the
   * view. It has to be an JSON Object.
   * @param stream A stream in which the HTML output will be forwarded.
   */
  void render(const std::string &view, const std::string &parameters,
              Stream &stream) override;

private:
  class Impl;

  /** Pointer to implementation */
  std::unique_ptr<Impl> m_impl;
};
}  // namespace complate
