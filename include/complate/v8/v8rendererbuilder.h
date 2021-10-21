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
  * @param sourceObj The complate JavaScript source bundle with the views.
  * @return Reference to this builder.
  */
  V8RendererBuilder &source(std::string sourceObj);

  /**
  * Pass a function that return the content of your views.js bundle.
  *
  * @param sourceCreator A function that return your complate JavaScript
  * source bundle with the views.
  * @return Reference to this builder.
  */
  V8RendererBuilder &source(SourceCreator sourceCreator);

  /**
  * Pass your bindings.
  *
  * @param bindingsObj Global variables which are available in every view.
  * @return Reference to this builder.
  */
  V8RendererBuilder &bindings(Object bindingsObj);

  /**
  * Pass a function that return your bindings.
  *
  * @param bindingsCreator A function that return your Global variables
  * which are available in every view.
  * @return Reference to this builder.
  */
  V8RendererBuilder &bindings(BindingsCreator bindingsCreator);

  /**
  * Pass your prototypes.
  *
  * @param prototypeList Prototypes for C++ classes to be supported via Proxy.
  * @return Reference to this builder.
  */
  V8RendererBuilder &prototypes(std::vector<Prototype> prototypeList);

  /**
  * Pass a function that return your prototypes.
  *
  * @param prototypesCreator A function that return Prototypes for C++ classes
  * to be supported via Proxy.
  * @return Reference to this builder.
  */
  V8RendererBuilder &prototypes(PrototypesCreator prototypesCreator);

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
