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
#include <complate/core/value.h>

#include "catch2/catch.hpp"

using namespace complate;
using namespace std;

TEST_CASE("Value", "[core]") {
  SECTION("traits") {
    REQUIRE(is_copy_constructible_v<Value>);
    REQUIRE(is_copy_assignable_v<Value>);
    REQUIRE(is_move_constructible_v<Value>);
    REQUIRE(is_move_assignable_v<Value>);
  }

  SECTION("undefined") {
    SECTION("is undefined") {
      const Value v;
      REQUIRE(v.is<Undefined>());
      REQUIRE(v.holds<Undefined>());
    }
  }

  SECTION("nullptr") {
    SECTION("is null") {
      const Value v = nullptr;
      REQUIRE(v.is<Null>());
      REQUIRE(v.holds<Null>());
    }
  }

  SECTION("bool") {
    SECTION("is compatible with true") {
      const Value v = true;
      REQUIRE(v.is<Bool>());
      REQUIRE(v.optional<Bool>() == true);
      REQUIRE(v.holds<Bool>());
      REQUIRE(v.exactly<Bool>() == true);
    }
  }

  SECTION("Numver") {
    SECTION("from int32_t") {
      const Value v = (int32_t)-32;
      REQUIRE(v.holds<Number>());
      REQUIRE(v.is<int32_t>());
      REQUIRE(v.optional<int32_t>() == -32);
    }

    SECTION("from uint32_t") {
      const Value v = (uint32_t)42;
      REQUIRE(v.holds<Number>());
      REQUIRE(v.is<uint32_t>());
      REQUIRE(v.optional<uint32_t>() == 42);
    }

    SECTION("from int64_t") {
      const Value v = (int64_t)42;
      REQUIRE(v.holds<Number>());
      REQUIRE(v.is<int64_t>());
      REQUIRE(v.optional<int64_t>() == 42);
    }

    SECTION("from double") {
      const Value v = (double)3.1415;
      REQUIRE(v.holds<Number>());
      REQUIRE(v.is<double>());
      REQUIRE(v.optional<double>() == 3.1415);
    }
  }

  SECTION("String") {
    SECTION("is String") {
      const Value v = String("Hello World!");
      REQUIRE(v.is<String>());
      REQUIRE(v.optional<String>().value() == "Hello World!");
      REQUIRE(v.holds<String>());
      REQUIRE(v.exactly<String>() == "Hello World!");
    }

    SECTION("from string") {
      const Value v = (string) "Hello World!";
      REQUIRE(v.is<string>());
      REQUIRE(v.optional<string>().value() == "Hello World!");
    }

    SECTION("from string_view") {
      const Value v = (string_view) "Hello World!";
      REQUIRE(v.is<string_view>());
      REQUIRE(v.optional<string_view>().value() == "Hello World!");
    }

    SECTION("from const char *") {
      const Value v = (const char *)"Hello World!";
      REQUIRE(v.is<string_view>());
      REQUIRE(v.optional<string_view>().value() == "Hello World!");
    }
  }

  SECTION("Array") {
    SECTION("is Array") {
      const Value v = Array{1, 2, 3};
      REQUIRE(v.is<Array>());
      REQUIRE(v.holds<Array>());
    }
  }

  SECTION("Object") {
    SECTION("is Object") {
      const Value v = Object{{"name", "john"}};
      REQUIRE(v.is<Object>());
      REQUIRE(v.holds<Object>());
    }
  }

  SECTION("Function") {
    SECTION("is Function") {
      const Value v = Function{[] { return Value(); }};
      REQUIRE(v.is<Function>());
      REQUIRE(v.holds<Function>());
    }

    SECTION("can be created without args") {
      const Value v = Function{[] { return -32; }};
      const Array args = Array{};
      const Value result = v.optional<Function>().value().apply(args);
      REQUIRE(result.optional<int32_t>().value() == -32);
    }

    SECTION("can be created with args") {
      const Value v = Function{[](const Array &args) {
        /*  add 1 to get the ultimate answer */
        return args.at(0).optional<int32_t>().value() + 1;
      }};
      const Array args = Array{41};
      const Value result = v.optional<Function>().value().apply(args);
      REQUIRE(result.optional<int32_t>().value() == 42);
    }
  }

  SECTION("optional<T>") {
    SECTION("is undefined, when optional is empty") {
      optional<string> opt;
      const Value v = opt;
      REQUIRE(v.holds<Undefined>());
    }

    SECTION("is undefined, when constructed with nullopt") {
      const Value v = nullopt;
      REQUIRE(v.holds<Undefined>());
    }

    SECTION("has value, when optional has value") {
      optional<string> opt = "Hello World!";
      const Value v = opt;
      REQUIRE(v.holds<String>());
      REQUIRE(v.optional<string>().value() == "Hello World!");
    }

    SECTION("with value of null") {
      const Value v = optional<Null>(nullptr);
      REQUIRE(v.holds<Null>());
    }

    SECTION("with value of bool") {
      const Value v = optional<bool>(false);
      REQUIRE(v.holds<Bool>());
    }

    SECTION("with value of int32_t") {
      const Value v = optional<int32_t>(23);
      REQUIRE(v.holds<Number>());
      REQUIRE(v.get<Number>().holds<int32_t>());
    }

    SECTION("with value of uint32_t") {
      const Value v = optional<uint32_t>(23);
      REQUIRE(v.holds<Number>());
      REQUIRE(v.get<Number>().holds<uint32_t>());
    }

    SECTION("with value of int64_t") {
      const Value v = optional<int64_t>(23);
      REQUIRE(v.holds<Number>());
      REQUIRE(v.get<Number>().holds<int64_t>());
    }

    SECTION("with value of double") {
      const Value v = optional<double>(23.7);
      REQUIRE(v.holds<Number>());
      REQUIRE(v.get<Number>().holds<double>());
    }

    SECTION("with value of Number") {
      const Value v = optional<Number>(23);
      REQUIRE(v.holds<Number>());
    }

    SECTION("with value of string") {
      const Value v = optional<string>("foo");
      REQUIRE(v.holds<String>());
      REQUIRE(v.get<String>().holds<string>());
    }

    SECTION("with value of string_view") {
      const Value v = optional<string_view>("foo");
      REQUIRE(v.holds<String>());
      REQUIRE(v.get<String>().holds<string_view>());
    }

    SECTION("with value of const char*") {
      const Value v = optional<const char *>("Hello World!");
      REQUIRE(v.holds<String>());
      REQUIRE(v.get<String>().holds<string>());
    }

    SECTION("with value of String") {
      const Value v = optional<String>("foo");
      REQUIRE(v.holds<String>());
    }

    SECTION("with value of Array") {
      const Value v = optional<Array>({1, 2, 3});
      REQUIRE(v.holds<Array>());
    }

    SECTION("with value of Object") {
      const Value v = optional<Object>(Object{{"foo", "bar"}});
      REQUIRE(v.holds<Object>());
    }

    SECTION("with value of Function") {
      const Value v = optional<Function>{[] { return 23; }};
      REQUIRE(v.holds<Function>());
    }

    SECTION("with value of Proxy") {
      const Value v =
          optional<Proxy>(in_place, "std::string", make_shared<string>("foo"));
      REQUIRE(v.holds<Proxy>());
    }

    SECTION("with value of ProxyWeak") {
      string text = "foo";
      const Value v = optional<ProxyWeak>(in_place, "std::string", &text);
      REQUIRE(v.holds<ProxyWeak>());
    }
  }

  SECTION("operator == and !=") {
    SECTION("check equality regardless of type") {
      const auto a =
          Object{{"name", string_view("Klaus")}, {"age", (int32_t)34}};
      const auto b = Object{{"name", string("Klaus")}, {"age", (uint32_t)34}};
      REQUIRE(a == b);
      REQUIRE_FALSE(a != b);
    }
  }
}
