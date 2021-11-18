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
#include "../../lib/v8/v8renderercontext.h"

#include "catch2/catch.hpp"
#include "testdata.h"

using namespace Catch::Matchers;
using namespace Catch::literals;
using namespace complate;
using namespace std;

TEST_CASE("V8PrototypeRegistry", "[v8]") {
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate *isolate = v8::Isolate::New(params);
  v8::Locker locker(isolate);
  v8::HandleScope hscope(isolate);
  auto context = v8::Context::New(isolate);
  v8::Context::Scope cscope(context);

  V8RendererContext rctx(isolate, Testdata::prototypes());
  auto &registry = rctx.prototypeRegistry();

  SECTION("unknown prototype return undefined for Proxy") {
    Proxy proxy("unknown", make_shared<int>(23));
    auto object = registry.newInstanceOf(proxy);
    REQUIRE(object->IsUndefined());
  }

  SECTION("unknown prototype return undefined for ProxyWeak") {
    int unknown = 23;
    ProxyWeak proxyWeak("unknown", &unknown);
    auto object = registry.newInstanceOf(proxyWeak);
    REQUIRE(object->IsUndefined());
  }

  SECTION("return object for Proxy") {
    auto text = make_shared<string>("foo");
    Proxy proxy("std::string", text);
    auto object = registry.newInstanceOf(proxy);
    REQUIRE(object->IsObject());
  }

  SECTION("return object for ProxyWeak") {
    string text = "foo";
    ProxyWeak proxyWeak("std::string", &text);
    auto object = registry.newInstanceOf(proxyWeak);
    REQUIRE(object->IsObject());
  }

  SECTION("object method can be called for Proxy") {
    auto text = make_shared<string>("foo");
    Proxy proxy("std::string", text);
    auto object = registry.newInstanceOf(proxy);
    REQUIRE(object->IsObject());

    auto pObject = object->ToObject(context).ToLocalChecked();

    auto pEmpty =
        pObject
            ->Get(context, v8::String::NewFromUtf8(isolate, "empty",
                                                   v8::NewStringType::kNormal)
                               .ToLocalChecked())
            .ToLocalChecked();
    REQUIRE(pEmpty->IsFunction());
    auto fEmpty = v8::Local<v8::Function>::Cast(pEmpty);

    auto rEmpty = fEmpty->Call(context, object, 0, nullptr).ToLocalChecked();
    REQUIRE(rEmpty->IsFalse());

    auto pClear =
        pObject
            ->Get(context, v8::String::NewFromUtf8(isolate, "clear",
                                                   v8::NewStringType::kNormal)
                               .ToLocalChecked())
            .ToLocalChecked();
    REQUIRE(pClear->IsFunction());
    auto fClear = v8::Local<v8::Function>::Cast(pClear);
    auto rClear = fClear->Call(context, pObject, 0, nullptr).ToLocalChecked();
    REQUIRE(rClear->IsUndefined());

    rEmpty = fEmpty->Call(context, pObject, 0, nullptr).ToLocalChecked();
    REQUIRE(rEmpty->IsTrue());
  }

  SECTION("object method can be called for ProxyWeak") {
    string text = "foo";
    ProxyWeak proxyWeak("std::string", &text);
    auto object = registry.newInstanceOf(proxyWeak);
    REQUIRE(object->IsObject());

    auto pObject = object->ToObject(context).ToLocalChecked();

    auto pEmpty = pObject->Get(
        context,
        v8::String::NewFromUtf8(isolate, "empty", v8::NewStringType::kNormal)
            .ToLocalChecked());
    REQUIRE(pEmpty.ToLocalChecked()->IsFunction());
    auto fEmpty = v8::Local<v8::Function>::Cast(pEmpty.ToLocalChecked());

    auto rEmpty = fEmpty->Call(context, object, 0, nullptr).ToLocalChecked();
    REQUIRE(rEmpty->IsFalse());

    auto pClear = pObject->Get(
        context,
        v8::String::NewFromUtf8(isolate, "clear", v8::NewStringType::kNormal)
            .ToLocalChecked());
    REQUIRE(pClear.ToLocalChecked()->IsFunction());
    auto fClear = v8::Local<v8::Function>::Cast(pClear.ToLocalChecked());
    auto rClear = fClear->Call(context, pObject, 0, nullptr).ToLocalChecked();
    REQUIRE(rClear->IsUndefined());

    rEmpty = fEmpty->Call(context, pObject, 0, nullptr).ToLocalChecked();
    REQUIRE(rEmpty->IsTrue());
  }

  SECTION("ProxyWeak property can be get") {
    string text = "foo";
    ProxyWeak proxyWeak("std::string", &text);
    auto object = registry.newInstanceOf(proxyWeak);
    REQUIRE(object->IsObject());

    auto pObject = object->ToObject(context).ToLocalChecked();
    auto length = pObject->Get(
        context,
        v8::String::NewFromUtf8(isolate, "length", v8::NewStringType::kNormal)
            .ToLocalChecked());
    REQUIRE(length.ToLocalChecked()->IsInt32());
    REQUIRE(
        length.ToLocalChecked()->ToInt32(context).ToLocalChecked()->Value() ==
        3);
  }

  SECTION("ProxyWeak property can be set") {
    string text = "foo";
    ProxyWeak proxyWeak("std::string", &text);
    auto object = registry.newInstanceOf(proxyWeak);
    REQUIRE(object->IsObject());

    auto pObject = object->ToObject(context).ToLocalChecked();
    pObject
        ->Set(
            context,
            v8::String::NewFromUtf8(isolate, "text", v8::NewStringType::kNormal)
                .ToLocalChecked(),
            v8::String::NewFromUtf8(isolate, "foobar",
                                    v8::NewStringType::kNormal)
                .ToLocalChecked())
        .ToChecked();

    REQUIRE(text == "foobar");
  }
}
