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
#include <complate/core/function.h>
#include <complate/core/value.h>

#include "catch2/catch.hpp"

using namespace std;
using namespace complate;

TEST_CASE("Function", "[core]") {
  SECTION("traits") {
    REQUIRE(is_copy_constructible_v<Function>);
    REQUIRE(is_copy_assignable_v<Function>);
    REQUIRE(is_move_constructible_v<Function>);
    REQUIRE(is_move_assignable_v<Function>);
  }

  SECTION("default constructed returns undefined") {
    Function f;
    REQUIRE(f.apply(Array{}).is<Undefined>());
  }

  SECTION("constructed without args") {
    bool called = false;
    Function f = Function{[&]() -> Value {
      called = true;
      return 23;
    }};

    SECTION("will call callback") {
      f.apply(Array{});
      REQUIRE(called);
    }

    SECTION("return value") {
      Value result = f.apply(Array{});
      REQUIRE(result == 23);
    }
  }

  SECTION("constructed with args") {
    const Array args = {"John", "Jane"};
    bool called = false;
    Array passed;
    Function f = Function{[&](const Array &arguments) -> Value {
      called = true;
      passed = arguments;
      return arguments.at(0);
    }};

    SECTION("will call callback") {
      f.apply(args);
      REQUIRE(called);
    }

    SECTION("will pass args") {
      f.apply(args);
      REQUIRE_FALSE(passed.empty());
      REQUIRE(passed == args);
    }

    SECTION("return value") {
      Value result = f.apply(args);
      REQUIRE(result == "John");
    }
  }

  SECTION("noop returns undefined") {
    Function f = Function::noop();
    REQUIRE(f.apply(Array{}).is<Undefined>());
  }

  SECTION("operator== is true (all functions are equal)") {
    auto a = Function{[]() -> Value { return 1; }};
    auto b = Function{[](const Array &args) -> Value { return args.empty(); }};
    REQUIRE(a == b);
  }

  SECTION("operator!= is false (all functions are equal)") {
    auto a = Function{[]() -> Value { return 1; }};
    auto b = Function{[](const Array &args) -> Value { return args.empty(); }};
    REQUIRE_FALSE(a != b);
  }
}
