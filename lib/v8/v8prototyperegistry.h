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
#include <complate/core/proxy.h>
#include <v8.h>

#include <map>
#include <string>
#include <memory>

namespace complate {

class V8PrototypeRegistry {
public:
  explicit V8PrototypeRegistry(v8::Isolate *isolate);

  void add(const Prototype &prototype);

  [[nodiscard]] v8::Local<v8::Value> newInstanceOf(const Proxy &proxy) const;
  [[nodiscard]] v8::Local<v8::Value> newInstanceOf(
      const ProxyWeak &proxyWeak) const;

private:
  using Template = v8::Persistent<v8::ObjectTemplate>;
  struct Entry {
    std::unique_ptr<Template> m_template;
    std::unique_ptr<Prototype> m_prototype;
  };
  v8::Isolate *m_isolate;
  std::map<std::string, Entry> m_entries;

  [[nodiscard]] v8::Local<v8::Value> newInstanceOf(const std::string &name,
                                                   void *ptr) const;

  static void methodCall(const v8::FunctionCallbackInfo<v8::Value> &info);

  static void getter(v8::Local<v8::String>,
                     const v8::PropertyCallbackInfo<v8::Value> &info);
  static void setter(v8::Local<v8::String> property, v8::Local<v8::Value> value,
                     const v8::PropertyCallbackInfo<void> &info);
};
}  // namespace complate
