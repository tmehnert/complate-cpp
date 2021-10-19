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
#include <complate/v8/v8platform.h>
#include <complate/v8/v8renderer.h>
#include <v8-version-string.h>

#include "catch2/catch.hpp"
#include "resources.h"

using namespace Catch::Matchers;
using namespace complate;
using namespace std;

TEST_CASE("V8Platform", "[v8]") {
  SECTION("version return something") {
    const auto version = string(V8Platform::version());
    REQUIRE_THAT(version, StartsWith(V8_VERSION_STRING));
  }

  SECTION("setFlags respected by the engine") {
    /*
     * Without V8Platform::setFlags("--use-strict") the engine would allow
     * sloppy JavaScript. With this flag enabled the engine raises this error.
     */
    const string sloppy = Resources::read("views.sloppy.js");
    REQUIRE_THROWS_AS(V8Renderer(sloppy), complate::Exception);
    REQUIRE_THROWS_WITH(V8Renderer(sloppy),
                        Contains("ReferenceError") && Contains("sloppy"));
  }
}