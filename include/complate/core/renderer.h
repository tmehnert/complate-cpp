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

#include <functional>
#include <memory>

#include "stream.h"
#include "value.h"

namespace complate {

/**
 * Renderer interface to get HTML output from a view and it's parameters.
 *
 * Several implementations included in this core libarary to alter a other
 * specific Renderer's behaviour, like `ReEvaluatingRenderer` does.
 * For actual Renderer implementations that really cause HTML output to be
 * generated look at the implementations for JavaScript Engines like QuickJS or
 * V8.
 *
 * When you want to bring another JavaScript Engine to complate or want to
 * change the behaviour of an existing Renderer, just subclass this interface.
 */
class Renderer {
public:
  virtual ~Renderer() = default;

  /** Creator for an Renderer, used by other renderers in this package. */
  using Creator = std::function<std::unique_ptr<Renderer>()>;

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
  virtual void render(const std::string &view, const Object &parameters,
                      Stream &stream) = 0;

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
  virtual void render(const std::string &view, const std::string &parameters,
                      Stream &stream) = 0;

  /**
   * Render a view to a String using an Object as parameters.
   *
   * @param view Name of the view you want to be rendered.
   * @param parameters The view Parameters aka 'the Model' which passed to the
   * view.
   * @return A string which contains the HTML output.
   */
  virtual std::string renderToString(const std::string &view,
                                     const Object &parameters) final;

  /**
   * Render a view to a String using a JSON string as parameters.
   *
   * @param view Name of the view you want to be rendered.
   * @param parameters The view Parameters aka 'the Model' which passed to the
   * view. It has to be an JSON Object.
   * @return A string which contains the HTML output.
   */
  virtual std::string renderToString(const std::string &view,
                                     const std::string &parameters) final;
};
}  // namespace complate
