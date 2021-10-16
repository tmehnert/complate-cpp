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
#include "complate/quickjs/quickjsrendererbuilder.h"

using namespace std;
using namespace complate;

class QuickJsRendererBuilder::Impl {
public:
  void withSource(string source) {
    m_source = move(source);
    m_sourceCreator = {};
  }

  void withSource(SourceCreator sourceCreator) {
    m_source = {};
    m_sourceCreator = move(sourceCreator);
  }

  void withBindings(Object bindings) {
    m_bindings = move(bindings);
    m_bindingsCreator = {};
  }

  void withBindings(BindingsCreator bindingsCreator) {
    m_bindings = {};
    m_bindingsCreator = move(bindingsCreator);
  }

  void withPrototypes(vector<Prototype> prototypes) {
    m_prototypes = move(prototypes);
    m_prototypesCreator = {};
  }

  void withPrototypes(PrototypesCreator prototypesCreator) {
    m_prototypes = vector<Prototype>();
    m_prototypesCreator = move(prototypesCreator);
  }

  [[nodiscard]] QuickJsRenderer build() const {
    return {(m_sourceCreator) ? invoke(m_sourceCreator) : m_source,
            (m_prototypesCreator) ? invoke(m_prototypesCreator) : m_prototypes,
            (m_bindingsCreator) ? invoke(m_bindingsCreator) : m_bindings};
  }

  [[nodiscard]] Renderer::Creator creator() const {
    return [*this] {
      return make_unique<QuickJsRenderer>(
          (m_sourceCreator) ? invoke(m_sourceCreator) : m_source,
          (m_prototypesCreator) ? invoke(m_prototypesCreator) : m_prototypes,
          (m_bindingsCreator) ? invoke(m_bindingsCreator) : m_bindings);
    };
  }

private:
  string m_source;
  SourceCreator m_sourceCreator;
  Object m_bindings;
  BindingsCreator m_bindingsCreator;
  vector<Prototype> m_prototypes;
  PrototypesCreator m_prototypesCreator;
};

QuickJsRendererBuilder::QuickJsRendererBuilder()
    : m_impl(make_unique<Impl>()) {}

QuickJsRendererBuilder::~QuickJsRendererBuilder() = default;

QuickJsRendererBuilder &QuickJsRendererBuilder::withSource(string source) {
  m_impl->withSource(move(source));
  return *this;
}

QuickJsRendererBuilder &QuickJsRendererBuilder::withSource(
    QuickJsRendererBuilder::SourceCreator sourceCreator) {
  m_impl->withSource(move(sourceCreator));
  return *this;
}

QuickJsRendererBuilder &QuickJsRendererBuilder::withBindings(Object bindings) {
  m_impl->withBindings(move(bindings));
  return *this;
}

QuickJsRendererBuilder &QuickJsRendererBuilder::withBindings(
    QuickJsRendererBuilder::BindingsCreator bindings) {
  m_impl->withBindings(move(bindings));
  return *this;
}

QuickJsRendererBuilder &QuickJsRendererBuilder::withPrototypes(
    std::vector<Prototype> prototypes) {
  m_impl->withPrototypes(move(prototypes));
  return *this;
}

QuickJsRendererBuilder &QuickJsRendererBuilder::withPrototypes(
    QuickJsRendererBuilder::PrototypesCreator prototypesCreator) {
  m_impl->withPrototypes(move(prototypesCreator));
  return *this;
}

QuickJsRenderer QuickJsRendererBuilder::build() const {
  return m_impl->build();
}

Renderer::Creator QuickJsRendererBuilder::creator() const {
  return m_impl->creator();
}
