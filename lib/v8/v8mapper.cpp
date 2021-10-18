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
#include "v8mapper.h"

#include "v8unmapper.h"

using namespace complate;
using namespace std;

V8Mapper::V8Mapper(v8::Isolate *isolate,
                   const std::vector<Prototype> &prototypes)
    : m_isolate(isolate), m_prototypeRegistry(isolate) {
  for (const Prototype &prototype : prototypes) {
    m_prototypeRegistry.add(prototype);
  }
}

// NOLINTNEXTLINE(misc-no-recursion)
v8::Local<v8::Object> V8Mapper::fromObject(const Object &object) {
  return fromObject(object, v8::Object::New(m_isolate));
}

// NOLINTNEXTLINE(misc-no-recursion)
v8::Local<v8::Object> V8Mapper::fromObject(const Object &object,
                                           v8::Local<v8::Object> parent) {
  auto context = m_isolate->GetCurrentContext();
  for (const auto &[k, v] : object) {
    v8::Local<v8::String> key = newInternalizedStringFrom(k);
    parent->Set(context, key, fromValue(v)).ToChecked();
  }
  return parent;
}

// NOLINTNEXTLINE(misc-no-recursion)
v8::Local<v8::Array> V8Mapper::fromArray(const Array &d) {
  auto context = m_isolate->GetCurrentContext();
  auto ar = v8::Array::New(m_isolate, (int)d.size());
  for (uint32_t i = 0; i < d.size(); ++i) {
    ar->Set(context, i, fromValue(d[i])).ToChecked();
  }
  return ar;
}

// NOLINTNEXTLINE(misc-no-recursion)
v8::Local<v8::Value> V8Mapper::fromValue(const Value &parameter) {
  if (parameter.holds<Null>()) {
    return valueFrom(parameter.exactly<Null>());
  } else if (parameter.holds<Bool>()) {
    return valueFrom(parameter.exactly<Bool>());
  } else if (parameter.holds<Number>()) {
    return valueFrom(parameter.exactly<Number>());
  } else if (parameter.holds<String>()) {
    return valueFrom(parameter.exactly<String>());
  } else if (parameter.holds<Array>()) {
    return fromArray(parameter.exactly<Array>());
  } else if (parameter.holds<Object>()) {
    return fromObject(parameter.exactly<Object>());
  } else if (parameter.holds<Function>()) {
    return fromFunction(parameter.exactly<Function>());
  } else if (parameter.holds<Proxy>()) {
    return fromProxy(parameter.exactly<Proxy>());
  } else if (parameter.holds<ProxyWeak>()) {
    return fromProxyWeak(parameter.exactly<ProxyWeak>());
  } else {
    return v8::Undefined(m_isolate);
  }
}

v8::Local<v8::Function> V8Mapper::fromFunction(const Function &d) {
  v8::Local<v8::External> fptr = v8::External::New(m_isolate, (void *)&d);
  return v8::FunctionTemplate::New(m_isolate, proxy, fptr)
      ->GetFunction(m_isolate->GetCurrentContext())
      .ToLocalChecked();
}

v8::Local<v8::Value> V8Mapper::fromProxy(const Proxy &proxy) {
  return m_prototypeRegistry.newInstanceOf(proxy);
}

v8::Local<v8::Value> V8Mapper::fromProxyWeak(const ProxyWeak &proxyWeak) {
  return m_prototypeRegistry.newInstanceOf(proxyWeak);
}

v8::Local<v8::Value> V8Mapper::valueFrom(const String &text) {
  auto sv = text.get<string_view>();
  return newStringFrom(sv.data(), sv.size());
}

v8::Local<v8::Value> V8Mapper::valueFrom(Null) { return v8::Null(m_isolate); }

v8::Local<v8::Value> V8Mapper::valueFrom(Bool d) {
  return v8::Boolean::New(m_isolate, d);
}

v8::Local<v8::Value> V8Mapper::valueFrom(const Number &number) {
  if (number.holds<int32_t>()) {
    return v8::Integer::New(m_isolate, number.exactly<int32_t>());
  } else if (number.holds<uint32_t>()) {
    return v8::Integer::NewFromUnsigned(m_isolate, number.exactly<uint32_t>());
  } else if (number.holds<int64_t>()) {
    return v8::BigInt::New(m_isolate, number.exactly<int64_t>());
  } else {
    return v8::Number::New(m_isolate, number.exactly<double>());
  }
}

v8::Local<v8::String> V8Mapper::newStringFrom(const char *str, size_t len) {
  return v8::String::NewFromUtf8(m_isolate, str, v8::NewStringType::kNormal,
                                 (int)len)
      .ToLocalChecked();
}

v8::Local<v8::String> V8Mapper::newInternalizedStringFrom(
    const std::string &str) {
  return v8::String::NewFromUtf8(m_isolate, str.c_str(),
                                 v8::NewStringType::kInternalized,
                                 (int)str.size())
      .ToLocalChecked();
}

void V8Mapper::proxy(const v8::FunctionCallbackInfo<v8::Value> &info) {
  V8Mapper mapper(info.GetIsolate(), {});
  V8Unmapper unmapper(info.GetIsolate());
  Array args;
  for (int i = 0; i < info.Length(); ++i) {
    args.emplace_back(unmapper.fromValue(info[i]));
  }

  auto intern = v8::Local<v8::External>::Cast(info.Data())->Value();
  auto fn = static_cast<Function *>(intern);
  info.GetReturnValue().Set(mapper.fromValue((*fn).apply(args)));
}
