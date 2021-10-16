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

#include <complate/core/prototype.h>
#include <complate/core/value.h>

#include <vector>

#include "quickjs.h"
#include "quickjsprototyperegistry.h"

namespace complate {

class QuickJsMapper {
public:
  explicit QuickJsMapper(JSContext *context,
                         const std::vector<Prototype> &prototypes);

  JSValue fromObject(const Object &object);
  JSValue fromObject(const Object &object, JSValue parent);
  JSValue fromArray(const Array &array);
  JSValue fromValue(const Value &value);
  JSValue fromFunction(const Function &func);
  JSValue fromProxy(const Proxy &proxy);
  JSValue fromProxyWeak(const ProxyWeak &proxyWeak);

private:
  JSContext *m_context;
  QuickJsPrototypeRegistry m_prototypeRegistry;

  inline JSValue valueFrom(Bool d);
  inline JSValue valueFrom(const Number &number);
  inline JSValue valueFrom(const String &text);

  static JSValue proxy(JSContext *ctx, JSValueConst this_val, int argc,
                       JSValueConst *argv, int magic, JSValue *data);
};
}  // namespace complate
