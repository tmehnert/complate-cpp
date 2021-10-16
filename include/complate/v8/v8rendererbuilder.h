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

#include "v8renderer.h"

namespace complate {

/**
* Builder for V8Renderer.
*
* This class should support constructing a V8Renderer or an
* Renderer::Creator for it.
 */
class V8RendererBuilder {
public:
  /** Creators to pass functions instead of values. */
  using SourceCreator = std::function<std::string()>;
  using BindingsCreator = std::function<Object()>;
  using PrototypesCreator = std::function<std::vector<Prototype>()>;

  V8RendererBuilder();
  ~V8RendererBuilder();

  /**
  * Pass the content of your views.js bundle.
  *
  * @param source The complate JavaScript source bundle with the views.
  * @return Reference to this builder.
   */
  V8RendererBuilder &withSource(std::string source);

  /**
  * Pass a function that return the content of your views.js bundle.
  *
  * @param sourceCreator A function that return your  complate JavaScript
  * source bundle with the views.
  * @return Reference to this builder.
   */
  V8RendererBuilder &withSource(SourceCreator sourceCreator);

  /**
  * Pass your bindings.
  *
  * @param bindings Global variables available in every view.
  * @return Reference to this builder.
   */
  V8RendererBuilder &withBindings(Object bindings);

  /**
  * Pass a function that return your bindings.
  *
  * @param bindings Global variables available in every view.
  * @return Reference to this builder.
   */
  V8RendererBuilder &withBindings(BindingsCreator bindingsCretor);

  /**
  * Pass your Prototypes.
  *
  * @param prototypes Prototypes for C++ classes to be supported via Proxy.
  * @return Reference to this builder.
   */
  V8RendererBuilder &withPrototypes(std::vector<Prototype> prototypes);

  /**
  * Pass a function that return your Prototypes.
  *
  * @param prototypes Prototypes for C++ classes to be supported via Proxy.
  * @return Reference to this builder.
   */
  V8RendererBuilder &withPrototypes(PrototypesCreator prototypesCreator);

  /** Build a renderer */
  [[nodiscard]] V8Renderer build() const;
  /** Create an Creator with can build a renderer */
  [[nodiscard]] Renderer::Creator creator() const;

private:
  class Impl;

  /** Pointer to implementation */
  std::unique_ptr<Impl> m_impl;
};

}  // namespace complate
