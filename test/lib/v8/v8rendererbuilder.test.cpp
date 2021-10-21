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
#include <complate/core/stringstream.h>
#include <complate/v8/v8rendererbuilder.h>

#include "catch2/catch.hpp"
#include "resources.h"
#include "testdata.h"

using namespace Catch::Matchers;
using namespace complate;
using namespace std;

// clang-format off
TEST_CASE("V8RendererBuilder", "[V8]") {
  auto stream = StringStream();

  SECTION("build with values") {

    auto renderer = V8RendererBuilder()
        .source(Resources::read("views.js"))
        .prototypes(Testdata::prototypes())
        .bindings(Testdata::bindings())
        .build();

    renderer.render("TodoList", Testdata::forTodoList(), stream);
    REQUIRE_THAT(stream.str(), Equals(Resources::read("todolist.html")));
  }

  SECTION("build with creators") {
    auto renderer = V8RendererBuilder()
        .source([]() { return Resources::read("views.js"); })
        .prototypes(Testdata::prototypes)
        .bindings(Testdata::bindings)
        .build();

    renderer.render("TodoList", Testdata::forTodoList(), stream);
    REQUIRE_THAT(stream.str(), Equals(Resources::read("todolist.html")));
  }

  SECTION("build a creator") {
    auto creator = V8RendererBuilder()
        .source(Resources::read("views.js"))
        .prototypes(Testdata::prototypes)
        .bindings(Testdata::bindings)
        .creator();

    creator()->render("TodoList", Testdata::forTodoList(), stream);
    REQUIRE_THAT(stream.str(), Equals(Resources::read("todolist.html")));
  }
}
// clang-format on
