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
#include "quickjsrenderercontext.h"

using namespace std;
using namespace complate;

QuickJsRendererContext::QuickJsRendererContext(
    JSContext *context, const vector<Prototype> &prototypes)
    : m_context(context),
      m_mapper(context),
      m_unmapper(context),
      m_prototypeRegistry(context) {
  JS_SetContextOpaque(m_context, this);
  for (const auto &prototype : prototypes) {
    m_prototypeRegistry.add(prototype);
  }
}

QuickJsRendererContext::~QuickJsRendererContext() {
  if (JS_GetContextOpaque(m_context) == this) {
    JS_SetContextOpaque(m_context, nullptr);
  }
}

QuickJsMapper& QuickJsRendererContext::mapper() { return m_mapper; }

QuickJsUnmapper& QuickJsRendererContext::unmapper() {
  return m_unmapper;
}

QuickJsPrototypeRegistry& QuickJsRendererContext::prototypeRegistry() {
  return m_prototypeRegistry;
}

QuickJsRendererContext* QuickJsRendererContext::get(JSContext* ctx) {
  return static_cast<QuickJsRendererContext *>(JS_GetContextOpaque(ctx));;
}
