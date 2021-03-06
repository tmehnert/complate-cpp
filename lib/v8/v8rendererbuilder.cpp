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
#include "complate/v8/v8rendererbuilder.h"

using namespace std;
using namespace complate;

class V8RendererBuilder::Impl {
public:
 void source(string sourceObj) {
   m_source = move(sourceObj);
   m_sourceCreator = {};
 }

 void source(SourceCreator sourceCreator) {
   m_source = {};
   m_sourceCreator = move(sourceCreator);
 }

 void bindings(Object bindingsObj) {
   m_bindings = move(bindingsObj);
   m_bindingsCreator = {};
 }

 void bindings(BindingsCreator bindingsCreator) {
   m_bindings = {};
   m_bindingsCreator = move(bindingsCreator);
 }

 void prototypes(vector<Prototype> prototypeList) {
   m_prototypes = move(prototypeList);
   m_prototypesCreator = {};
 }

 void prototypes(PrototypesCreator prototypesCreator) {
   m_prototypes = vector<Prototype>();
   m_prototypesCreator = move(prototypesCreator);
 }

 [[nodiscard]] V8Renderer build() const {
   return {(m_sourceCreator) ? invoke(m_sourceCreator) : m_source,
           (m_prototypesCreator) ? invoke(m_prototypesCreator) : m_prototypes,
           (m_bindingsCreator) ? invoke(m_bindingsCreator) : m_bindings};
 }

 [[nodiscard]] unique_ptr<V8Renderer> unique() const {
   return make_unique<V8Renderer>(
       (m_sourceCreator) ? invoke(m_sourceCreator) : m_source,
       (m_prototypesCreator) ? invoke(m_prototypesCreator) : m_prototypes,
       (m_bindingsCreator) ? invoke(m_bindingsCreator) : m_bindings);
 }

 [[nodiscard]] Renderer::Creator creator() const {
   return [*this] {
     return make_unique<V8Renderer>(
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

V8RendererBuilder::V8RendererBuilder()
   : m_impl(make_unique<Impl>()) {}

V8RendererBuilder::~V8RendererBuilder() = default;

V8RendererBuilder &V8RendererBuilder::source(string sourceObj) {
  m_impl->source(move(sourceObj));
 return *this;
}

V8RendererBuilder &V8RendererBuilder::source(
   V8RendererBuilder::SourceCreator sourceCreator) {
  m_impl->source(move(sourceCreator));
 return *this;
}

V8RendererBuilder &V8RendererBuilder::bindings(Object bindingsObj) {
  m_impl->bindings(move(bindingsObj));
 return *this;
}

V8RendererBuilder &V8RendererBuilder::bindings(
   V8RendererBuilder::BindingsCreator bindingsCreator) {
  m_impl->bindings(move(bindingsCreator));
 return *this;
}

V8RendererBuilder &V8RendererBuilder::prototypes(
   std::vector<Prototype> prototypeList) {
  m_impl->prototypes(move(prototypeList));
 return *this;
}

V8RendererBuilder &V8RendererBuilder::prototypes(
   V8RendererBuilder::PrototypesCreator prototypesCreator) {
  m_impl->prototypes(move(prototypesCreator));
 return *this;
}

V8Renderer V8RendererBuilder::build() const {
 return m_impl->build();
}

unique_ptr<V8Renderer> complate::V8RendererBuilder::unique() const {
  return m_impl->unique();
}

Renderer::Creator V8RendererBuilder::creator() const {
 return m_impl->creator();
}
