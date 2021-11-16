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
#include "v8unmapper.h"

using namespace complate;
using namespace std;

V8Unmapper::V8Unmapper(v8::Isolate *isolate) : m_isolate(isolate) {}

// NOLINTNEXTLINE(misc-no-recursion)
Value V8Unmapper::fromValue(v8::Local<v8::Value> value) {
  auto context = m_isolate->GetCurrentContext();

  if (value->IsString()) {
    return fromString(value->ToString(context).ToLocalChecked());
  } else if (value->IsNumber()) {
    if (value->IsUint32()) {
      return value->ToUint32(context).ToLocalChecked()->Value();
    } else if (value->IsInt32()) {
      return value->ToInt32(context).ToLocalChecked()->Value();
    } else {
      return value->ToNumber(context).ToLocalChecked()->Value();
    }
  } else if (value->IsBigInt()) {
    return value->ToBigInt(context).ToLocalChecked()->Int64Value();
  } else if (value->IsBoolean()) {
    return value->BooleanValue(context).ToChecked();
  } else if (value->IsNull()) {
    return nullptr;
  } else if (value->IsArray()) {
    return fromArray(value.As<v8::Array>());
  } else if (value->IsObject()) {
    return fromObject(value.As<v8::Object>());
  }

  return {};
}

// NOLINTNEXTLINE(misc-no-recursion)
Array V8Unmapper::fromArray(v8::Local<v8::Array> arr) {
  Array array;
  array.reserve(arr->Length());

  for (uint32_t i = 0; i < arr->Length(); i++) {
    array.push_back(fromValue(arr->Get(i)));
  }

  return array;
}

// NOLINTNEXTLINE(misc-no-recursion)
Object V8Unmapper::fromObject(v8::Local<v8::Object> obj) {
  auto pfilter = (v8::PropertyFilter)(v8::ONLY_ENUMERABLE | v8::SKIP_SYMBOLS);
  auto keyconv = v8::KeyConversionMode::kConvertToString;
  auto context = m_isolate->GetCurrentContext();
  Object object;

  v8::Local<v8::Array> props;
  props = obj->GetOwnPropertyNames(context, pfilter, keyconv).ToLocalChecked();
  for (uint32_t i = 0; i < props->Length(); i++) {
    auto k = props->Get(i);
    string key = fromString(k->ToString(context).ToLocalChecked());
    object.emplace(key, fromValue(obj->Get(k)));
  }

  return object;
}

string V8Unmapper::fromString(v8::Local<v8::String> str) {
  v8::String::Utf8Value utf8(m_isolate, str);
  string s;
  s.assign(*utf8, utf8.length());
  return s;
}
