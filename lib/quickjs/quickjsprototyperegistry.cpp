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
#include "quickjsprototyperegistry.h"

#include "quickjsrenderercontext.h"

using namespace std;
using namespace complate;

QuickJsPrototypeRegistry::QuickJsPrototypeRegistry(JSContext *context)
    : m_context(context) {}

void QuickJsPrototypeRegistry::add(const Prototype &prototype) {
  JSClassID classId = 0;
  JS_NewClassID(&classId);

  JSRuntime *runtime = JS_GetRuntime(m_context);
  JSClassDef classDef{};
  classDef.class_name = js_strdup(m_context, prototype.name().c_str());
  int rc = JS_NewClass(runtime, classId, &classDef);
  if (rc < 0) {
    throw Exception("could not register JSClassDef of Prototype'");
  }

  JSValue tmpl = JS_NewObject(m_context);
  auto proto = make_unique<Prototype>(prototype);

  auto functions = make_unique<vector<JSCFunctionListEntry>>();
  int16_t i = 1;
  for (const auto &method : proto->methods()) {
    functions->push_back(entry(method.name(), i));
    JS_SetPropertyUint32(m_context, tmpl, i, JS_MKPTR(1, (void *)&method));
    i++;
  }

  for (const auto &property : proto->properties()) {
    functions->push_back(getset(property.name(), i));
    JS_SetPropertyUint32(m_context, tmpl, i, JS_MKPTR(1, (void *)&property));
    i++;
  }

  JS_SetPropertyFunctionList(m_context, tmpl, functions->data(),
                             (int)functions->size());
  JS_SetClassProto(m_context, classId, tmpl);
  m_entries.emplace(prototype.name(),
                    Entry{classId, move(proto), move(functions)});
}

JSValue QuickJsPrototypeRegistry::newInstanceOf(const Proxy &proxy) const {
  return newInstanceOf(proxy.name(), proxy.ptr().get());
}

JSValue QuickJsPrototypeRegistry::newInstanceOf(
    const ProxyWeak &proxyWeak) const {
  return newInstanceOf(proxyWeak.name(), proxyWeak.ptr());
}

JSValue QuickJsPrototypeRegistry::newInstanceOf(const string &name,
                                                void *ptr) const {
  auto it = m_entries.find(name);
  if (it != m_entries.cend()) {
    JSValue object = JS_NewObjectClass(m_context, (int)it->second.m_classId);
    JS_SetPropertyUint32(m_context, object, 0, JS_MKPTR(1, ptr));
    return object;
  } else {
    return JS_UNDEFINED;
  }
}

JSValue QuickJsPrototypeRegistry::methodCall(JSContext *ctx, JSValue this_val,
                                             int argc, JSValue *argv,
                                             int magic) {
  auto rctx = QuickJsRendererContext::get(ctx);
  Array args;
  for (int i = 0; i < argc; ++i) {
    args.emplace_back(rctx->unmapper().fromValue(argv[i]));
  }
  void *proxy = JS_VALUE_GET_PTR(JS_GetPropertyUint32(ctx, this_val, 0));
  JSValue m = JS_GetPropertyUint32(ctx, this_val, magic);
  auto method = static_cast<Method *>(JS_VALUE_GET_PTR(m));

  return rctx->mapper().fromValue(method->apply(proxy, args));
}

JSCFunctionListEntry QuickJsPrototypeRegistry::entry(const string &name,
                                                     int16_t num) {
  JSCFunctionListEntry e{};
  e.name = name.c_str();
  e.prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
  e.def_type = JS_DEF_CFUNC;
  e.magic = num;
  e.u.func.length = 8;
  e.u.func.cproto = JS_CFUNC_generic_magic;
  e.u.func.cfunc.generic_magic = methodCall;
  return e;
}

JSCFunctionListEntry QuickJsPrototypeRegistry::getset(const string &name,
                                                      int16_t num) {
  JSCFunctionListEntry e{};
  e.name = name.c_str();
  e.prop_flags = JS_PROP_CONFIGURABLE;
  e.def_type = JS_DEF_CGETSET_MAGIC;
  e.magic = num;
  e.u.getset.get.getter_magic = getter;
  e.u.getset.set.setter_magic = setter;
  return e;
}

JSValue QuickJsPrototypeRegistry::getter(JSContext *ctx, JSValue this_val,
                                         int magic) {
  auto rctx = QuickJsRendererContext::get(ctx);

  void *proxy = JS_VALUE_GET_PTR(JS_GetPropertyUint32(ctx, this_val, 0));
  JSValue m = JS_GetPropertyUint32(ctx, this_val, magic);
  auto property = static_cast<Property *>(JS_VALUE_GET_PTR(m));

  return rctx->mapper().fromValue(property->get(proxy));
}

JSValue QuickJsPrototypeRegistry::setter(JSContext *ctx, JSValue this_val,
                                         JSValue val, int magic) {
  auto rctx = QuickJsRendererContext::get(ctx);

  void *proxy = JS_VALUE_GET_PTR(JS_GetPropertyUint32(ctx, this_val, 0));
  JSValue m = JS_GetPropertyUint32(ctx, this_val, magic);
  auto property = static_cast<Property *>(JS_VALUE_GET_PTR(m));

  property->set(proxy, rctx->unmapper().fromValue(val));
  return JS_UNDEFINED;
}
