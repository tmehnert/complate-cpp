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
#include "v8prototyperegistry.h"

#include <functional>

#include "v8renderercontext.h"

using namespace std;
using namespace complate;

V8PrototypeRegistry::V8PrototypeRegistry(v8::Isolate *isolate)
    : m_isolate(isolate) {}

void V8PrototypeRegistry::add(const Prototype &prototype) {
  v8::Locker locker(m_isolate);
  v8::HandleScope scope(m_isolate);
  auto tmpl = v8::ObjectTemplate::New(m_isolate);
  tmpl->SetInternalFieldCount(1);

  auto proto = make_unique<Prototype>(prototype);
  for (const auto &method : proto->methods()) {
    tmpl->Set(m_isolate, method.name().c_str(),
              v8::FunctionTemplate::New(
                  m_isolate, methodCall,
                  v8::External::New(m_isolate, (void *)&method)));
  }

  for (const auto &property : proto->properties()) {
    tmpl->SetNativeDataProperty(
        v8::String::NewFromUtf8(m_isolate, property.name().c_str(),
                                v8::NewStringType::kInternalized)
            .ToLocalChecked(),
        getter, setter, v8::External::New(m_isolate, (void *)&property));
  }

  m_entries.emplace(prototype.name(),
                    Entry{make_unique<Template>(m_isolate, tmpl), move(proto)});
}

v8::Local<v8::Value> V8PrototypeRegistry::newInstanceOf(
    const Proxy &proxy) const {
  V8RendererContext::get(m_isolate)->proxyHolder().add(proxy);
  return newInstanceOf(proxy.name(), proxy.ptr().get());
}

v8::Local<v8::Value> V8PrototypeRegistry::newInstanceOf(
    const ProxyWeak &proxyWeak) const {
  return newInstanceOf(proxyWeak.name(), proxyWeak.ptr());
}

v8::Local<v8::Value> V8PrototypeRegistry::newInstanceOf(const string &name,
                                                        void *ptr) const {
  auto it = m_entries.find(name);
  if (it != m_entries.cend()) {
    auto object = it->second.m_template->Get(m_isolate)
                      ->NewInstance(m_isolate->GetCurrentContext())
                      .ToLocalChecked();
    object->SetInternalField(0, v8::External::New(m_isolate, ptr));
    return object;
  } else {
    return v8::Undefined(m_isolate);
  }
}

void V8PrototypeRegistry::methodCall(
    const v8::FunctionCallbackInfo<v8::Value> &info) {
  auto rctx = V8RendererContext::get(info.GetIsolate());

  Array args;
  for (int i = 0; i < info.Length(); ++i) {
    args.emplace_back(rctx->unmapper().fromValue(info[i]));
  }

  auto pptr =
      v8::Local<v8::External>::Cast(info.This()->GetInternalField(0))->Value();
  auto data = v8::Local<v8::External>::Cast(info.Data())->Value();
  auto method = static_cast<Method *>(data);

  info.GetReturnValue().Set(rctx->mapper().fromValue(method->apply(pptr, args)));
}

void V8PrototypeRegistry::getter(
    v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info) {
  auto rctx = V8RendererContext::get(info.GetIsolate());

  auto pptr =
      v8::Local<v8::External>::Cast(info.This()->GetInternalField(0))->Value();
  auto data = v8::Local<v8::External>::Cast(info.Data())->Value();
  auto prop = static_cast<Property *>(data);

  info.GetReturnValue().Set(rctx->mapper().fromValue(prop->get(pptr)));
}

void V8PrototypeRegistry::setter(v8::Local<v8::String>,
                                 v8::Local<v8::Value> value,
                                 const v8::PropertyCallbackInfo<void> &info) {
  auto rctx = V8RendererContext::get(info.GetIsolate());

  auto pptr =
      v8::Local<v8::External>::Cast(info.This()->GetInternalField(0))->Value();
  auto data = v8::Local<v8::External>::Cast(info.Data())->Value();
  auto prop = static_cast<Property *>(data);

  prop->set(pptr, rctx->unmapper().fromValue(value));
}
