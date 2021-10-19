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
#include "v8helper.h"

using namespace std;

v8::Local<v8::String> V8Helper::newString(v8::Isolate *isolate,
                                          string_view str) {
  return v8::String::NewFromUtf8(isolate, str.data(),
                                 v8::NewStringType::kNormal, (int)str.size())
      .ToLocalChecked();
}
