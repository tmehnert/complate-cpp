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

#include <vector>

#include "v8mapper.h"
#include "v8prototyperegistry.h"
#include "v8unmapper.h"
#include "v8proxyholder.h"

namespace complate {

class V8RendererContext {
public:
  static const uint32_t DATA_SLOT = 0;

  explicit V8RendererContext(v8::Isolate *isolate,
                             const std::vector<Prototype> &prototypes = {});

  ~V8RendererContext();

  [[nodiscard]] V8Mapper &mapper();
  [[nodiscard]] V8Unmapper &unmapper();
  [[nodiscard]] V8PrototypeRegistry &prototypeRegistry();
  [[nodiscard]] V8ProxyHolder &proxyHolder();

  static V8RendererContext *get(v8::Isolate *isolate);

private:
  v8::Isolate *m_isolate;
  V8Mapper m_mapper;
  V8Unmapper m_unmapper;
  V8PrototypeRegistry m_prototypeRegistry;
  V8ProxyHolder m_proxyHolder;
};
}  // namespace complate
