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
#include <v8.h>

#include <vector>

#include "v8prototyperegistry.h"

namespace complate {

class V8Mapper {
public:
  explicit V8Mapper(v8::Isolate *isolate,
                    const std::vector<Prototype> &prototypes);

  v8::Local<v8::Object> fromObject(const Object &object);
  v8::Local<v8::Object> fromObject(const Object &object,
                                   v8::Local<v8::Object> parent);
  v8::Local<v8::Array> fromArray(const Array &d);
  v8::Local<v8::Value> fromValue(const Value &parameter);
  v8::Local<v8::Function> fromFunction(const Function &d);
  v8::Local<v8::Value> fromProxy(const Proxy &proxy);
  v8::Local<v8::Value> fromProxyWeak(const ProxyWeak &proxyWeak);

private:
  v8::Isolate *m_isolate;
  V8PrototypeRegistry m_prototypeRegistry;

  inline v8::Local<v8::Value> valueFrom(Null);
  inline v8::Local<v8::Value> valueFrom(Bool d);
  inline v8::Local<v8::Value> valueFrom(const Number &number);
  inline v8::Local<v8::Value> valueFrom(const String &text);

  inline v8::Local<v8::String> newStringFrom(const char *str, size_t len);

  static void proxy(const v8::FunctionCallbackInfo<v8::Value> &info);
};
}  // namespace complate
