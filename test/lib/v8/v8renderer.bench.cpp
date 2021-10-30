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
#include <complate/v8/v8renderer.h>

#include "catch2/catch.hpp"
#include "noopstream.h"
#include "resources.h"
#include "testdata.h"

using namespace complate;
using namespace std;

TEST_CASE("V8RendererBenchmark", "[v8][.benchmark]") {
  V8Renderer renderer(Resources::read("views.js"), Testdata::prototypes(),
                      Testdata::bindings());
  auto stream = NoopStream();

  SECTION("rendering TodoList") {
    auto view = "TodoList";
    Object parameters;

    parameters = Testdata::forTodoListRenderBenchmark(0);
    BENCHMARK("map with no todos") {
      renderer.render(view, parameters, stream);
    };

    parameters = Testdata::forTodoListRenderBenchmark(10);
    BENCHMARK("map with 10 todos") {
      renderer.render(view, parameters, stream);
    };

    parameters = Testdata::forTodoListRenderBenchmark(100);
    BENCHMARK("map with 100 todos") {
      renderer.render(view, parameters, stream);
    };
  }
}