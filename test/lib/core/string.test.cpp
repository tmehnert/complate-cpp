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
#include <complate/core/string.h>

#include <map>

#include "catch2/catch.hpp"

using namespace complate;
using namespace std;

TEST_CASE("String", "[core]") {
  SECTION("traits") {
    REQUIRE(is_copy_constructible_v<String>);
    REQUIRE(is_copy_assignable_v<String>);
    REQUIRE(is_move_constructible_v<String>);
    REQUIRE(is_move_assignable_v<String>);
  }

  SECTION("can be used as map key") {
    map<String, int> m = {{"Little Doo", 1}};
    m.emplace("John", 2);
    m["Jane"] = 3;
    REQUIRE(m.at("Little Doo") == 1);
    REQUIRE(m.at("John") == 2);
    REQUIRE(m["Jane"] == 3);
  }

  SECTION("construct default") {
    String t;
    SECTION("holds string_view") { REQUIRE(t.holds<string_view>()); }
    SECTION("is an empty string") { REQUIRE(t == ""); }
  }

  SECTION("construct with string") {
    String t = string("Hello World!");
    SECTION("holds string") { REQUIRE(t.holds<string>()); }
    SECTION("will be initialized") { REQUIRE(t == "Hello World!"); }
    SECTION("get as string") { REQUIRE(t.get<string>() == "Hello World!"); }
    SECTION("get as string_view") {
      REQUIRE(t.get<string_view>() == "Hello World!");
    }
  }

  SECTION("construct with string_view") {
    String t = string_view("Hello World!");
    SECTION("holds string_view") { REQUIRE(t.holds<string_view>()); }
    SECTION("will be initialized") { REQUIRE(t == "Hello World!"); }
    SECTION("get as string") { REQUIRE(t.get<string>() == "Hello World!"); }
    SECTION("get as string_view") {
      REQUIRE(t.get<string_view>() == "Hello World!");
    }
  }

  SECTION("construct with const char *") {
    String t = (const char *)"Hello World!";
    SECTION("holds string") { REQUIRE(t.holds<string>()); }
    SECTION("will be initialized") { REQUIRE(t == "Hello World!"); }
  }

  SECTION("compare equal") {
    SECTION("when holds string") {
      const String t = string("I'm equal");
      SECTION("compared against string") {
        REQUIRE(t == string("I'm equal"));
        REQUIRE(t != string("I'm not equal"));
        REQUIRE_FALSE(t != string("I'm equal"));
        REQUIRE_FALSE(t == string("I'm not equal"));
      }
      SECTION("compared against string_view") {
        REQUIRE(t == string_view("I'm equal"));
        REQUIRE(t != string_view("I'm not equal"));
        REQUIRE_FALSE(t != string_view("I'm equal"));
        REQUIRE_FALSE(t == string_view("I'm not equal"));
      }
      SECTION("compared against const char *") {
        REQUIRE(t == "I'm equal");
        REQUIRE(t != "I'm not equal");
        REQUIRE_FALSE(t != "I'm equal");
        REQUIRE_FALSE(t == "I'm not equal");
      }
    }

    SECTION("when holds string_view") {
      const String t = string_view("I'm equal");
      SECTION("compared against string") {
        REQUIRE(t == string("I'm equal"));
        REQUIRE(t != string("I'm not equal"));
        REQUIRE_FALSE(t != string("I'm equal"));
        REQUIRE_FALSE(t == string("I'm not equal"));
      }
      SECTION("compared against string_view") {
        REQUIRE(t == string_view("I'm equal"));
        REQUIRE(t != string_view("I'm not equal"));
        REQUIRE_FALSE(t != string_view("I'm equal"));
        REQUIRE_FALSE(t == string_view("I'm not equal"));
      }
      SECTION("compared against const char *") {
        REQUIRE(t == "I'm equal");
        REQUIRE(t != "I'm not equal");
        REQUIRE_FALSE(t != "I'm equal");
        REQUIRE_FALSE(t == "I'm not equal");
      }
    }
  }
}
