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
#include "../../../lib/quickjs/quickjshelper.h"

#include <complate/core/exception.h>

#include "catch2/catch.hpp"
#include "quickjs.h"
#include "resources.h"

using namespace Catch::Matchers;
using namespace std;

TEST_CASE("QuickJsHelper", "[quickjs]") {
  JSRuntime *runtime = JS_NewRuntime();
  JSContext *context = JS_NewContext(runtime);
  JSValue v = JS_UNINITIALIZED;

  SECTION("evaluate") {
    SECTION("can evaluate the views.js bundle") {
      const string bundle = Resources::read("views.js");
      QuickJsHelper::evaluate(context, bundle);
      v = QuickJsHelper::getFunction(context, "render");
      REQUIRE(JS_IsFunction(context, v));
    }

    SECTION("throws complate::Exception when source is malformed") {
      const string malformed = Resources::read("views.js.malformed");
      REQUIRE_THROWS_AS(QuickJsHelper::evaluate(context, malformed),
                        complate::Exception);
      REQUIRE_THROWS_WITH(QuickJsHelper::evaluate(context, malformed),
                          Contains("SyntaxError"));
    }
  }

  SECTION("getFunction") {
    SECTION("return the function") {
      QuickJsHelper::evaluate(context, "function foo() {};");
      v = QuickJsHelper::getFunction(context, "foo");
      REQUIRE(JS_IsFunction(context, v));
    }

    SECTION("throws complate::Exception when error occured in eval") {
      const string malformed = "foo /&=/!{D<";
      QuickJsHelper::evaluate(context, "function foo() {};");
      REQUIRE_THROWS_AS(QuickJsHelper::getFunction(context, malformed),
                        complate::Exception);
      REQUIRE_THROWS_WITH(QuickJsHelper::getFunction(context, malformed),
                          Contains("SyntaxError"));
    }

    SECTION("throws complate::Exception when name is not a function") {
      QuickJsHelper::evaluate(context, "const foo = 3;");
      REQUIRE_THROWS_AS(QuickJsHelper::getFunction(context, "foo"),
                        complate::Exception);
      REQUIRE_THROWS_WITH(QuickJsHelper::getFunction(context, "foo"),
                          Contains("Is not a function"));
    }

    SECTION("throws complate::Exception when name is undefined") {
      REQUIRE_THROWS_AS(QuickJsHelper::getFunction(context, "foo"),
                        complate::Exception);
      REQUIRE_THROWS_WITH(QuickJsHelper::getFunction(context, "foo"),
                          Contains("ReferenceError: 'foo' is not defined"));
    }
  }

  if (!JS_IsUninitialized(v)) {
    JS_FreeValue(context, v);
  }
  JS_FreeContext(context);
  JS_FreeRuntime(runtime);
}