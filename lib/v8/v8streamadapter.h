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

#include <complate/core/stream.h>
#include <v8.h>

namespace complate {

class V8StreamAdapter {
public:
  explicit V8StreamAdapter(v8::Isolate* isolate);

  v8::Local<v8::Object> adapterFor(Stream& stream);

private:
  static const std::size_t FAST_UTF8_BUFLEN = 2048;
  v8::Isolate* m_isolate;
  v8::Persistent<v8::ObjectTemplate> m_template;

  static void write(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void writeln(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void flush(const v8::FunctionCallbackInfo<v8::Value>& args);

  static inline Stream* stream(const v8::FunctionCallbackInfo<v8::Value>& args);
};
}  // namespace complate
