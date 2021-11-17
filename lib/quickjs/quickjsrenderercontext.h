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

#include "quickjsmapper.h"
#include "quickjsprototyperegistry.h"
#include "quickjsunmapper.h"
#include "quickjsproxyholder.h"

namespace complate {

class QuickJsRendererContext {
public:
  explicit QuickJsRendererContext(
      JSContext *context, const std::vector<Prototype> &prototypes = {});
  ~QuickJsRendererContext();

  [[nodiscard]] QuickJsMapper &mapper();
  [[nodiscard]] QuickJsUnmapper &unmapper();
  [[nodiscard]] QuickJsPrototypeRegistry &prototypeRegistry();
  [[nodiscard]] QuickJsProxyHolder &proxyHolder();

  static QuickJsRendererContext *get(JSContext *ctx);

private:
  JSContext *m_context;
  QuickJsMapper m_mapper;
  QuickJsUnmapper m_unmapper;
  QuickJsPrototypeRegistry m_prototypeRegistry;
  QuickJsProxyHolder m_proxyHolder;
};
}  // namespace complate
