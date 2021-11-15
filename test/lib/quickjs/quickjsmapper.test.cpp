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

TEST_CASE("QuickJsMapper", "[quickjs]") {
  JSRuntime *runtime = JS_NewRuntime();
  JSContext *context = JS_NewContext(runtime);
  QuickJsRendererContext rctx(context, Testdata::prototypes());
  auto &mapper = rctx.mapper();
  JSValue v;

  SECTION("fromObject") {
    SECTION("convert properties") {
      const Object obj = {
          {"name", "Klaus"}, {"age", 23}, {"lucky", true}, {"car", nullptr}};
      v = mapper.fromObject(obj);
      REQUIRE(JS_IsString(JS_GetPropertyStr(context, v, "name")));
      REQUIRE(JS_IsNumber(JS_GetPropertyStr(context, v, "age")));
      REQUIRE(JS_IsBool(JS_GetPropertyStr(context, v, "lucky")));
      REQUIRE(JS_IsNull(JS_GetPropertyStr(context, v, "car")));
    }

    SECTION("convert nested arrays") {
      const Object obj = {{"items", Array{"Eating", -23, true, nullptr}}};
      v = mapper.fromObject(obj);
      JSValue items = JS_GetPropertyStr(context, v, "items");
      CHECK(JS_IsArray(context, items));
      CHECK(JS_IsString(JS_GetPropertyUint32(context, items, 0)));
      CHECK(JS_IsNumber(JS_GetPropertyUint32(context, items, 1)));
      CHECK(JS_IsBool(JS_GetPropertyUint32(context, items, 2)));
      CHECK(JS_IsNull(JS_GetPropertyUint32(context, items, 3)));
      JS_FreeValue(context, items);
    }

    SECTION("convert nested objects") {
      const Object obj = {
          {"address", Object{{"street", "Hamburger Str."}, {"houseno", 155}}}};
      v = mapper.fromObject(obj);
      JSValue address = JS_GetPropertyStr(context, v, "address");
      CHECK(JS_IsObject(address));
      CHECK(JS_IsString(JS_GetPropertyStr(context, address, "street")));
      CHECK(JS_IsNumber(JS_GetPropertyStr(context, address, "houseno")));
      JS_FreeValue(context, address);
    }
  }

  SECTION("fromArray") {
    SECTION("convert nested objects") {
      const Array arr = {Object{{"name", "John"}}, Object{{"name", "Jane"}}};

      v = mapper.fromArray(arr);
      REQUIRE(JS_IsArray(context, v));
      JSValue john = JS_GetPropertyUint32(context, v, 0);
      CHECK(JS_IsObject(john));
      CHECK(JS_IsString(JS_GetPropertyStr(context, john, "name")));
      JSValue jane = JS_GetPropertyUint32(context, v, 0);
      CHECK(JS_IsObject(jane));
      CHECK(JS_IsString(JS_GetPropertyStr(context, jane, "name")));
      JS_FreeValue(context, john);
      JS_FreeValue(context, jane);
    }
  }

  SECTION("fromValue") {
    SECTION("convert null") {
      v = mapper.fromValue(Null());
      REQUIRE(JS_IsNull(v));
    }

    SECTION("convert true") {
      v = mapper.fromValue(true);
      REQUIRE(JS_IsBool(v));
      REQUIRE(JS_ToBool(context, v) != 0);
    }

    SECTION("convert false") {
      v = mapper.fromValue(false);
      REQUIRE(JS_IsBool(v));
      REQUIRE(JS_ToBool(context, v) == 0);
    }

    SECTION("convert int32") {
      v = mapper.fromValue((int32_t)-23);
      REQUIRE(JS_IsNumber(v));
      int32_t val = 0;
      REQUIRE(JS_ToInt32(context, &val, v) == 0);
      REQUIRE(val == -23);
    }

    SECTION("convert uint32") {
      v = mapper.fromValue((uint32_t)42);
      REQUIRE(JS_IsNumber(v));
      uint32_t val = 0;
      REQUIRE(JS_ToUint32(context, &val, v) == 0);
      REQUIRE(val == 42);
    }

    SECTION("convert int64") {
      v = mapper.fromValue((int64_t)-112344567879);
      REQUIRE(JS_IsNumber(v));
      int64_t val = 0;
      REQUIRE(JS_ToInt64(context, &val, v) == 0);
      REQUIRE(val == -112344567879);
    }

    SECTION("convert double") {
      v = mapper.fromValue((double)3.14);
      REQUIRE(JS_IsNumber(v));
      int tag = JS_VALUE_GET_TAG(v);
      REQUIRE(JS_TAG_IS_FLOAT64(tag));
      double val = 0;
      REQUIRE(JS_ToFloat64(context, &val, v) == 0);
      REQUIRE(val == 3.14_a);
    }

    SECTION("convert const char* nullptr") {
      const char *str = nullptr;
      v = mapper.fromValue(str);
      REQUIRE(JS_IsNull(v));
    }

    SECTION("convert String") {
      v = mapper.fromValue(String("Hello string!"));
      REQUIRE(JS_IsString(v));
      REQUIRE_THAT(JS_ToCString(context, v), Equals("Hello string!"));
    }

    SECTION("convert Array") {
      v = mapper.fromValue(Array{0, "foo", nullptr, false});
      REQUIRE(JS_IsArray(context, v));
      REQUIRE(JS_IsNumber(JS_GetPropertyUint32(context, v, 0)));
      REQUIRE(JS_IsString(JS_GetPropertyUint32(context, v, 1)));
      REQUIRE(JS_IsNull(JS_GetPropertyUint32(context, v, 2)));
      REQUIRE(JS_IsBool(JS_GetPropertyUint32(context, v, 3)));
    }

    SECTION("convert Object") {
      v = mapper.fromValue(Object{{"a", 16}, {"b", "hi"}});
      REQUIRE(JS_IsObject(v));
      REQUIRE(JS_IsNumber(JS_GetPropertyStr(context, v, "a")));
      REQUIRE(JS_IsString(JS_GetPropertyStr(context, v, "b")));
    }

    SECTION("convert Proxy") {
      v = mapper.fromValue(Proxy{"std::string", make_shared<string>()});
      REQUIRE(JS_IsObject(v));
      JSValue method = JS_GetPropertyStr(context, v, "clear");
      REQUIRE(JS_IsFunction(context, method));
      JS_FreeValue(context, method);
    }

    SECTION("convert ProxyWeak") {
      string object = "foo";
      v = mapper.fromValue(ProxyWeak{"std::string", &object});
      REQUIRE(JS_IsObject(v));
      JSValue method = JS_GetPropertyStr(context, v, "clear");
      REQUIRE(JS_IsFunction(context, method));
      JS_FreeValue(context, method);
    }
  }

  JS_FreeValue(context, v);
  JS_FreeContext(context);
  JS_FreeRuntime(runtime);
}