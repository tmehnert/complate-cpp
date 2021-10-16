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
#include "../../../lib/quickjs/quickjsunmapper.h"

#include "catch2/catch.hpp"
#include "quickjs.h"

using namespace complate;
using namespace std;

TEST_CASE("QuickJsUnmapper", "[quickjs]") {
  JSRuntime *runtime = JS_NewRuntime();
  JSContext *context = JS_NewContext(runtime);
  QuickJsUnmapper unmapper(context);
  JSValue value;

  SECTION("fromValue") {
    SECTION("unmap undefined") {
      value = JS_UNDEFINED;
      const Value unmapped = unmapper.fromValue(value);
      REQUIRE(unmapped.holds<Undefined>());
    }

    SECTION("unmap null") {
      value = JS_NULL;
      const Value unmapped = unmapper.fromValue(value);
      REQUIRE(unmapped.holds<Null>());
    }

    SECTION("unmap true") {
      value = JS_TRUE;
      const Value unmapped = unmapper.fromValue(value);
      REQUIRE(unmapped.holds<bool>());
      REQUIRE(unmapped.exactly<bool>() == true);
    }

    SECTION("unmap false") {
      value = JS_FALSE;
      const Value unmapped = unmapper.fromValue(value);
      REQUIRE(unmapped.holds<bool>());
      REQUIRE(unmapped.exactly<bool>() == false);
    }

    SECTION("unmap int32_t") {
      value = JS_NewInt32(context, -32);
      const Value unmapped = unmapper.fromValue(value);
      const Number number = unmapped.exactly<Number>();
      /* The unmapper is unable to determine if it's an signed or unsigned, so i
       * use int64_t to be safe */
      REQUIRE(number.holds<int64_t>());
      REQUIRE(number.exactly<int64_t>() == -32);
      /* But the value is compatible with uint32_t */
      REQUIRE(number.optional<int32_t>() == -32);
    }

    SECTION("unmap uint32_t") {
      value = JS_NewUint32(context, 815);
      const Value unmapped = unmapper.fromValue(value);
      const Number number = unmapped.exactly<Number>();
      /* The unmapper is unable to determine if it's an signed or unsigned, so i
       * use int64_t to be safe */
      REQUIRE(number.holds<int64_t>());
      REQUIRE(number.exactly<int64_t>() == 815);
      /* But the value is compatible with uint32_t */
      REQUIRE(number.optional<uint32_t>() == 815);
    }

    SECTION("unmap int64_t") {
      value = JS_NewInt64(context, -123000);
      const Value unmapped = unmapper.fromValue(value);
      const Number number = unmapped.exactly<Number>();
      REQUIRE(number.holds<int64_t>());
      REQUIRE(number.exactly<int64_t>() == -123000);
    }

    SECTION("unmap double") {
      value = JS_NewFloat64(context, 3.1415);
      const Value unmapped = unmapper.fromValue(value);
      const Number number = unmapped.exactly<Number>();
      REQUIRE(number.holds<double>());
      REQUIRE(number.exactly<double>() == Approx(3.1415));
    }

    SECTION("unmap text") {
      value = JS_NewString(context, "Hello World!");
      const Value unmapped = unmapper.fromValue(value);
      REQUIRE(unmapped.holds<String>());
      REQUIRE(unmapped.exactly<String>() == "Hello World!");
    }
  }

  JS_FreeValue(context, value);
  JS_FreeContext(context);
  JS_FreeRuntime(runtime);
}