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
#include "quickjsmapper.h"

#include "quickjsunmapper.h"

using namespace complate;
using namespace std;

QuickJsMapper::QuickJsMapper(JSContext *context,
                             const std::vector<Prototype> &prototypes)
    : m_context(context), m_prototypeRegistry(context) {
  for (const Prototype &prototype : prototypes) {
    m_prototypeRegistry.add(prototype);
  }
}

// NOLINTNEXTLINE(misc-no-recursion)
JSValue QuickJsMapper::fromObject(const Object &object) {
  return fromObject(object, JS_NewObject(m_context));
}

// NOLINTNEXTLINE(misc-no-recursion)
JSValue QuickJsMapper::fromObject(const Object &object, JSValue parent) {
  for (const auto &[k, v] : object) {
    JS_SetPropertyStr(m_context, parent, k.c_str(), fromValue(v));
  }
  return parent;
}

// NOLINTNEXTLINE(misc-no-recursion)
JSValue QuickJsMapper::fromArray(const Array &array) {
  JSValue arr = JS_NewArray(m_context);
  for (uint32_t i = 0; i < array.size(); ++i) {
    JS_SetPropertyUint32(m_context, arr, i, fromValue(array[i]));
  }
  return arr;
}

// NOLINTNEXTLINE(misc-no-recursion)
JSValue QuickJsMapper::fromValue(const Value &value) {
  if (value.holds<Null>()) {
    return JS_NULL;
  } else if (value.holds<Bool>()) {
    return valueFrom(value.exactly<Bool>());
  } else if (value.holds<Number>()) {
    return valueFrom(value.exactly<Number>());
  } else if (value.holds<String>()) {
    return valueFrom(value.exactly<String>());
  } else if (value.holds<Array>()) {
    return fromArray(value.exactly<Array>());
  } else if (value.holds<Object>()) {
    return fromObject(value.exactly<Object>());
  } else if (value.holds<Function>()) {
    return fromFunction(value.exactly<Function>());
  } else if (value.holds<Proxy>()) {
    return fromProxy(value.exactly<Proxy>());
  } else if (value.holds<ProxyWeak>()) {
    return fromProxyWeak(value.exactly<ProxyWeak>());
  } else {
    return JS_UNDEFINED;
  }
}

JSValue QuickJsMapper::fromFunction(const Function &func) {
  JSValue v = JS_NewObject(m_context);
  JS_SetOpaque(v, (void *)&func);
  JSValue f = JS_NewCFunctionData(m_context, proxy, 8, 0, 1, &v);
  JS_SetPropertyUint32(m_context, f, 0, v);
  return f;
}

JSValue QuickJsMapper::fromProxy(const Proxy &proxy) {
  return m_prototypeRegistry.newInstanceOf(proxy);
}

JSValue QuickJsMapper::fromProxyWeak(const ProxyWeak &proxyWeak) {
  return m_prototypeRegistry.newInstanceOf(proxyWeak);
}

JSValue QuickJsMapper::valueFrom(Bool d) { return JS_NewBool(m_context, d); }

JSValue QuickJsMapper::valueFrom(const Number &number) {
  if (number.holds<int32_t>()) {
    return JS_NewInt32(m_context, number.exactly<int32_t>());
  } else if (number.holds<uint32_t>()) {
    return JS_NewUint32(m_context, number.exactly<uint32_t>());
  } else if (number.holds<int64_t>()) {
    return JS_NewInt64(m_context, number.exactly<int64_t>());
  } else if (number.holds<double>()) {
    return JS_NewFloat64(m_context, number.exactly<double>());
  } else {
    return JS_UNDEFINED;
  }
}

JSValue QuickJsMapper::valueFrom(const String &text) {
  auto sv = text.get<string_view>();
  return JS_NewStringLen(m_context, sv.data(), sv.size());
}

JSValue QuickJsMapper::proxy(JSContext *ctx, JSValue, int argc, JSValue *argv,
                             int, JSValue *data) {
  QuickJsMapper mapper(ctx, {});
  QuickJsUnmapper unmapper(ctx);
  Array args;
  for (int i = 0; i < argc; ++i) {
    args.emplace_back(unmapper.fromValue(argv[i]));
  }
  auto fn = static_cast<Function *>(JS_GetOpaque(*data, 1));
  return mapper.fromValue((*fn).apply(args));
}
