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

#include "renderer.h"

namespace complate {

/**
 * Renderer which creates a Renderer for every thread calls render.
 *
 * The creation of the Renderer is delayed until a thread calls `render()`
 * fot the first time.
 *
 * @attention Due limitations of `thread_local` it' its only be possible to
 * create one `ThreadLocalRenderer` per application.
 * @attention The underlying Renderer will not be deleted after deconstructing
 * the `ThreadLocalRenderer`. In order to free up Resources you have to call
 * reset() from the thread who called `render()`.
 */
class ThreadLocalRenderer : public Renderer {
public:
  /**
   * Constructs a ReEvaluatingRenderer
   *
   * @param creator This function is stored and will be used to create
   * Renderer's.
   */
  explicit ThreadLocalRenderer(Creator creator);

  ~ThreadLocalRenderer() override;

  /**
   * Using a thread local Renderer to render a view to a Stream using an Object
   * as parameters.
   *
   * The arguments will be forwarded to the Renderer created by Creator.
   * The Renderer instance will be stored for subsequent calls from calling
   * thread.
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
   * Using a thread local Renderer to render a view to a Stream using a JSON
   * string as parameters.
   *
   * The arguments will be forwarded to the Renderer created by Creator.
   * After the that the newly created Renderer will be deleted.
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

  /** Delete the underlying thread local Renderer instance */
  void reset();

private:
  class Impl;

  /** Pointer to implementation */
  std::unique_ptr<Impl> m_impl;
};
}  // namespace complate
