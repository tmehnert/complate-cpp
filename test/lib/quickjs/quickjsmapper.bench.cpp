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
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "../../../lib/quickjs/quickjsrenderercontext.h"

#include "catch2/catch.hpp"
#include "quickjs.h"
#include "testdata.h"

using namespace complate;
using namespace std;

TEST_CASE("QuickjsMapperBenchmark", "[quickjs][.benchmark]") {
  JSRuntime *runtime = JS_NewRuntime();
  JSContext *context = JS_NewContext(runtime);
  QuickJsRendererContext rctx(context, Testdata::prototypes());
  auto &mapper = rctx.mapper();

  const Object parameters = Testdata::forMapperBenchmark();

  BENCHMARK("convert parameters") {
    JSValue v = mapper.fromObject(parameters);
    JS_FreeValue(context, v);
  };

  JS_FreeContext(context);
  JS_FreeRuntime(runtime);
}