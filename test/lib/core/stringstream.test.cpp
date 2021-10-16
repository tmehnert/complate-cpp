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

#include "catch2/catch.hpp"

using namespace Catch::Matchers;
using namespace complate;
using namespace std;

TEST_CASE("StringStream", "[core]") {
  auto stream = StringStream();

  SECTION("is constructed with an empty string") {
    REQUIRE(stream.str().empty());
  }

  SECTION("write appending to string") {
    stream.write("7 chars", 7);
    REQUIRE_THAT(stream.str(), Equals("7 chars"));
    stream.write(" + 11 chars", 11);
    REQUIRE_THAT(stream.str(), Equals("7 chars + 11 chars"));
  }

  SECTION("writeln appending to string with newlines") {
    stream.writeln("7 chars", 7);
    REQUIRE_THAT(stream.str(), Equals("7 chars\n"));
    stream.writeln(" + 11 chars", 11);
    REQUIRE_THAT(stream.str(), Equals("7 chars\n + 11 chars\n"));
  }

  SECTION("flush does nothing with the string") {
    stream.write("untouched", 9);
    stream.flush();
    REQUIRE_THAT(stream.str(), Equals("untouched"));
  }
}