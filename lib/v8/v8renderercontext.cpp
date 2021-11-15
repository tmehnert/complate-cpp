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
#include "v8renderercontext.h"

using namespace std;
using namespace complate;

V8RendererContext::V8RendererContext(v8::Isolate* isolate,
                                     const vector<Prototype>& prototypes)
    : m_isolate(isolate),
      m_mapper(isolate),
      m_unmapper(isolate),
      m_prototypeRegistry(isolate) {
  m_isolate->SetData(DATA_SLOT, this);
  for (const auto& prototype : prototypes) {
    m_prototypeRegistry.add(prototype);
  }
}

V8RendererContext::~V8RendererContext() {
  if (m_isolate->GetData(DATA_SLOT) == this) {
    m_isolate->SetData(DATA_SLOT, nullptr);
  }
}

V8Mapper& V8RendererContext::mapper() { return m_mapper; }

V8Unmapper& V8RendererContext::unmapper() { return m_unmapper; }

V8PrototypeRegistry& V8RendererContext::prototypeRegistry() {
  return m_prototypeRegistry;
}

V8RendererContext* V8RendererContext::get(v8::Isolate* isolate) {
  return static_cast<V8RendererContext*>(
      isolate->GetData(V8RendererContext::DATA_SLOT));
}
