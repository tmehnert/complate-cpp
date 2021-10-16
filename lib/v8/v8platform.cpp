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
#include <complate/v8/v8platform.h>
#include <libplatform/libplatform.h>
#include <v8.h>

using namespace complate;
using namespace std;

class V8Platform::Impl {
public:
  Impl() : m_platform(v8::platform::NewDefaultPlatform()) {
    v8::V8::InitializePlatform(m_platform.get());
    v8::V8::Initialize();
  }

  static void setFlags(string_view flags) {
    v8::V8::SetFlagsFromString(flags.data(), (int)flags.length());
  }

  static string_view getVersion() { return v8::V8::GetVersion(); }

private:
  unique_ptr<v8::Platform> m_platform;
};

V8Platform::V8Platform() : m_impl(make_unique<Impl>()) {}
complate::V8Platform::~V8Platform() = default;

void V8Platform::setFlags(string_view flags) { Impl::setFlags(flags); }

string_view complate::V8Platform::version() { return Impl::getVersion(); }
