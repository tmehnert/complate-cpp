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
#include <complate/core/exception.h>
#include <complate/core/stringstream.h>
#include <complate/v8/v8renderer.h>

#include "catch2/catch.hpp"
#include "resources.h"
#include "testdata.h"

using namespace Catch::Matchers;
using namespace complate;
using namespace std;

TEST_CASE("V8Renderer", "[v8]") {
  auto stream = StringStream();

  SECTION("constructor") {
    SECTION("throws complate::Exception when render function is not defined") {
      REQUIRE_THROWS_AS(V8Renderer(""), complate::Exception);
      REQUIRE_THROWS_WITH(V8Renderer(""),
                          Contains("ReferenceError") && Contains("render"));
    }

    SECTION("throws complate::Exception when source bundle is malformed") {
      const string malformed = Resources::read("views.js.malformed");
      REQUIRE_THROWS_AS(V8Renderer(malformed), complate::Exception);
      REQUIRE_THROWS_WITH(V8Renderer(malformed), Contains("SyntaxError"));
    }
  }

  SECTION("render map parameters") {
    SECTION("generate expected output for TodoList") {
      V8Renderer renderer(Resources::read("views.js"), Testdata::prototypes(),
                          Testdata::bindings());
      renderer.render("TodoList", Testdata::forTodoList(), stream);
      REQUIRE_THAT(stream.str(), Equals(Resources::read("todolist.html")));
    }

    SECTION("throws complate::Exception when view is not defined") {
      V8Renderer renderer(Resources::read("views.js"), Testdata::prototypes(),
                          Testdata::bindings());
      const Object parameters;
      REQUIRE_THROWS_AS(renderer.render("MissingView", parameters, stream),
                        complate::Exception);
      REQUIRE_THROWS_WITH(
          renderer.render("MissingView", parameters, stream),
          Contains("unknown view macro") && Contains("MissingView"));
    }
  }

  SECTION("renderToString map parameters") {
    SECTION("generate expected output for TodoList") {
      V8Renderer renderer(Resources::read("views.js"), Testdata::prototypes(),
                          Testdata::bindings());
      const string html =
          renderer.renderToString("TodoList", Testdata::forTodoList());
      REQUIRE_THAT(html, Equals(Resources::read("todolist.html")));
    }
  }

  SECTION("render json parameters") {
    SECTION("generate expected output for TodoList") {
      V8Renderer renderer(Resources::read("views.js"), Testdata::prototypes(),
                          Testdata::bindings());
      renderer.render("TodoList", Testdata::forTodoListViewAsJson(), stream);
      REQUIRE_THAT(stream.str(), Equals(Resources::read("todolist.html")));
    }

    SECTION("throws complate::Exception when view is not defined") {
      V8Renderer renderer(Resources::read("views.js"), Testdata::prototypes(),
                          Testdata::bindings());
      const string parameters = "{}";
      REQUIRE_THROWS_AS(renderer.render("MissingView", parameters, stream),
                        complate::Exception);
      REQUIRE_THROWS_WITH(
          renderer.render("MissingView", parameters, stream),
          Contains("unknown view macro") && Contains("MissingView"));
    }

    SECTION("throws complate::Exception if parameters is not an json object") {
      V8Renderer renderer(Resources::read("views.js"));
      const string parameters;
      REQUIRE_THROWS_AS(renderer.render("TodoList", parameters, stream),
                        complate::Exception);
      REQUIRE_THROWS_WITH(renderer.render("TodoList", parameters, stream),
                          Contains("SyntaxError") && Contains("parameters"));
    }
  }

  SECTION("renderToString json parameters") {
    SECTION("generate expected output for TodoList") {
      V8Renderer renderer(Resources::read("views.js"), Testdata::prototypes(),
                          Testdata::bindings());
      const string html = renderer.renderToString(
          "TodoList", Testdata::forTodoListViewAsJson());
      REQUIRE_THAT(html, Equals(Resources::read("todolist.html")));
    }
  }
}
