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
#include "../../../lib/quickjs/quickjsrenderercontext.h"

#include <complate/core/prototypebuilder.h>

#include "catch2/catch.hpp"
#include "quickjs.h"
#include "testdata.h"

using namespace Catch::Matchers;
using namespace Catch::literals;
using namespace complate;
using namespace std;

TEST_CASE("QuickJsPrototypeRegistry", "[quickjs]") {
  JSRuntime *runtime = JS_NewRuntime();
  JSContext *context = JS_NewContext(runtime);
  QuickJsRendererContext rctx(context, Testdata::prototypes());
  auto &registry = rctx.prototypeRegistry();
  JSValue v;

  registry.add(Testdata::prototypeForStdString());

  SECTION("unknown prototype return undefined for Proxy") {
    Proxy proxy("unknown", make_shared<int>(23));
    v = registry.newInstanceOf(proxy);
    REQUIRE(JS_IsUndefined(v));
  }

  SECTION("unknown prototype return undefined for ProxyWeak") {
    int unknown = 23;
    ProxyWeak proxyWeak("unknown", &unknown);
    v = registry.newInstanceOf(proxyWeak);
    REQUIRE(JS_IsUndefined(v));
  }

  SECTION("return object for Proxy") {
    auto text = make_shared<string>("foo");
    Proxy proxy("std::string", text);
    v = registry.newInstanceOf(proxy);
    REQUIRE(JS_IsObject(v));
  }

  SECTION("return object for ProxyWeak") {
    string text = "foo";
    ProxyWeak proxyWeak("std::string", &text);
    v = registry.newInstanceOf(proxyWeak);
    REQUIRE(JS_IsObject(v));
  }

  SECTION("Proxy method can be called") {
    auto text = make_shared<string>("foo");
    Proxy proxy("std::string", text);
    v = registry.newInstanceOf(proxy);
    REQUIRE(JS_IsObject(v));

    JSValue mEmpty = JS_GetPropertyStr(context, v, "empty");
    REQUIRE(JS_IsFunction(context, mEmpty));
    JSValue rEmpty = JS_Call(context, mEmpty, v, 0, nullptr);
    REQUIRE(JS_IsBool(rEmpty));
    REQUIRE(JS_ToBool(context, rEmpty) == false);

    JSValue mClear = JS_GetPropertyStr(context, v, "clear");
    REQUIRE(JS_IsFunction(context, mClear));
    JSValue rClear = JS_Call(context, mClear, v, 0, nullptr);
    REQUIRE(JS_IsUndefined(rClear));

    JS_FreeValue(context, rEmpty);
    rEmpty = JS_Call(context, mEmpty, v, 0, nullptr);
    REQUIRE(JS_IsBool(rEmpty));
    REQUIRE(JS_ToBool(context, rEmpty) == true);

    JS_FreeValue(context, rEmpty);
    JS_FreeValue(context, mEmpty);
    JS_FreeValue(context, rClear);
    JS_FreeValue(context, mClear);
  }

  SECTION("ProxyWeak method can be called") {
    string text = "foo";
    ProxyWeak proxyWeak("std::string", &text);
    v = registry.newInstanceOf(proxyWeak);
    REQUIRE(JS_IsObject(v));

    JSValue mEmpty = JS_GetPropertyStr(context, v, "empty");
    REQUIRE(JS_IsFunction(context, mEmpty));
    JSValue rEmpty = JS_Call(context, mEmpty, v, 0, nullptr);
    REQUIRE(JS_IsBool(rEmpty));
    REQUIRE(JS_ToBool(context, rEmpty) == false);

    JSValue mClear = JS_GetPropertyStr(context, v, "clear");
    REQUIRE(JS_IsFunction(context, mClear));
    JSValue rClear = JS_Call(context, mClear, v, 0, nullptr);
    REQUIRE(JS_IsUndefined(rClear));

    JS_FreeValue(context, rEmpty);
    rEmpty = JS_Call(context, mEmpty, v, 0, nullptr);
    REQUIRE(JS_IsBool(rEmpty));
    REQUIRE(JS_ToBool(context, rEmpty) == true);

    JS_FreeValue(context, rEmpty);
    JS_FreeValue(context, mEmpty);
    JS_FreeValue(context, rClear);
    JS_FreeValue(context, mClear);
  }

  SECTION("ProxyWeak property can be get") {
    string text = "foo";
    ProxyWeak proxyWeak("std::string", &text);
    v = registry.newInstanceOf(proxyWeak);
    REQUIRE(JS_IsObject(v));

    JSValue mLength = JS_GetPropertyStr(context, v, "length");
    REQUIRE(JS_IsNumber(mLength));
    int32_t length = 0;
    JS_ToInt32(context, &length, mLength);
    REQUIRE(length == 3);

    JS_FreeValue(context, mLength);
  }

  SECTION("ProxyWeak property can be set") {
    string text = "foo";
    ProxyWeak proxyWeak("std::string", &text);
    v = registry.newInstanceOf(proxyWeak);
    REQUIRE(JS_IsObject(v));

    JSValue newText = JS_NewString(context, "foobar");
    JS_SetPropertyStr(context, v, "text", newText);
    REQUIRE(text == "foobar");
  }

  JS_FreeValue(context, v);
  JS_FreeContext(context);
  JS_FreeRuntime(runtime);
}