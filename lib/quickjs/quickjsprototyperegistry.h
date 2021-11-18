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
#include <quickjs.h>

#include <map>
#include <string>
#include <vector>
#include <memory>

namespace complate {

class QuickJsPrototypeRegistry {
public:
  explicit QuickJsPrototypeRegistry(JSContext *context);

  void add(const Prototype &prototype);

  [[nodiscard]] JSValue newInstanceOf(const Proxy &proxy) const;
  [[nodiscard]] JSValue newInstanceOf(const ProxyWeak &proxyWeak) const;

private:
  struct Entry;
  JSContext *m_context;
  std::map<std::string, Entry> m_entries;

  [[nodiscard]] JSValue newInstanceOf(const std::string &name, void *ptr) const;

  static JSValue methodCall(JSContext *ctx, JSValueConst this_val, int argc,
                            JSValueConst *argv, int magic);
  static JSCFunctionListEntry entry(const std::string &name, int16_t num);

  static JSCFunctionListEntry getset(const std::string &name, int16_t num);

  static JSValue getter(JSContext *ctx, JSValueConst this_val, int magic);
  static JSValue setter(JSContext *ctx, JSValueConst this_val, JSValueConst val,
                        int magic);

  struct Entry {
    JSClassID m_classId;
    std::unique_ptr<Prototype> m_prototype;
    std::unique_ptr<std::vector<JSCFunctionListEntry>> m_function;
  };
};
}  // namespace complate
