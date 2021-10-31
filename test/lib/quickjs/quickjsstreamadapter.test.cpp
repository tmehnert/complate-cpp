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
#include "../../../lib/quickjs/quickjsstreamadapter.h"

#include "../../../lib/quickjs/quickjshelper.h"
#include "catch2/catch.hpp"
#include "stream.mock.h"
#include "quickjs.h"

using namespace trompeloeil;
using namespace complate;
using namespace std;

TEST_CASE("QuickJsStreamAdapter", "[quickjs]") {
  JSRuntime *runtime = JS_NewRuntime();
  JSContext *context = JS_NewContext(runtime);
  JSValue argv[1];

  auto stream = StreamMock();
  QuickJsStreamAdapter streamAdapter(context);
  QuickJsHelper::evaluate(context, R"(
      var testWrite = function(stream) {
        stream.write('7 chars');
      }
      var testWriteln = function(stream) {
        stream.writeln('8 chars*');
      }
      var testFlush = function(stream) {
        stream.flush();
      }
  )");

  SECTION("forward write to stream") {
    const string expectedStr = "7 chars";
    REQUIRE_CALL(stream, write(eq<const char *>(expectedStr), eq<int>(7)))
        .TIMES(1);

    argv[0] = streamAdapter.adapterFor(stream);
    JSValue testWrite = QuickJsHelper::getFunction(context, "testWrite");
    JSValue rc = JS_Call(context, testWrite, JS_UNDEFINED, 1, argv);
    CHECK(JS_IsException(rc) == 0);
    JS_FreeValue(context, testWrite);
  }

  SECTION("forward writeln to stream") {
    const string expectedStr = "8 chars*";
    REQUIRE_CALL(stream, writeln(eq<const char *>(expectedStr), eq<int>(8)))
        .TIMES(1);

    argv[0] = streamAdapter.adapterFor(stream);
    JSValue testWriteln = QuickJsHelper::getFunction(context, "testWriteln");
    JSValue rc = JS_Call(context, testWriteln, JS_UNDEFINED, 1, argv);
    CHECK(JS_IsException(rc) == 0);
    JS_FreeValue(context, testWriteln);
  }

  SECTION("forward flush to stream") {
    REQUIRE_CALL(stream, flush()).TIMES(1);

    argv[0] = streamAdapter.adapterFor(stream);
    JSValue testFlush = QuickJsHelper::getFunction(context, "testFlush");
    JSValue rc = JS_Call(context, testFlush, JS_UNDEFINED, 1, argv);
    CHECK(JS_IsException(rc) == 0);
    JS_FreeValue(context, testFlush);
  }

  JS_FreeValue(context, argv[0]);
  JS_FreeContext(context);
  JS_FreeRuntime(runtime);
}
