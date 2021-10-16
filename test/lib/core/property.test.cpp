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
#include <complate/core/property.h>

#include "catch2/catch.hpp"
#include "prototypetestclass.h"

using namespace complate;
using namespace std;

TEST_CASE("Property", "[core]") {
  PrototypeTestClass testClass("foobar");

  SECTION("constructed by copy") {
    Property property1("prop", &PrototypeTestClass::strReturnNoArgs);
    Property property2 = property1;  // NOLINT - I want to copy to test copy
    REQUIRE(property1.get(&testClass) == "foobar");
    REQUIRE(property2.get(&testClass) == "foobar");
  }

  SECTION("constructed with lambdas") {
    Property property(
        "prop",
        [](void *p) -> Value {
          return static_cast<PrototypeTestClass *>(p)->strReturnNoArgs();
        },
        [](void *p, const Value &value) {
          static_cast<PrototypeTestClass *>(p)->setReturnString(value);
        });
    REQUIRE(property.get(&testClass) == "foobar");
    property.set(&testClass, "baz");
    REQUIRE(property.get(&testClass) == "baz");
  }

  SECTION("constructed with getter to member function pointer") {
    Property property("prop", &PrototypeTestClass::strReturnNoArgs);
    REQUIRE(property.get(&testClass) == "foobar");
  }

  SECTION("constructed with non-const getter to member function pointer") {
    Property property("prop", &PrototypeTestClass::nonConstGetter);
    REQUIRE(property.get(&testClass) == "foobar");
  }

  SECTION("constructed with getter and setter to member function pointer") {
    Property property("prop", &PrototypeTestClass::strReturnNoArgs,
                      &PrototypeTestClass::setReturnString);
    REQUIRE(property.get(&testClass) == "foobar");
    property.set(&testClass, "baz");
    REQUIRE(property.get(&testClass) == "baz");
  }

  SECTION(
      "constructed with non-const getter and setter to member function "
      "pointer") {
    Property property("prop", &PrototypeTestClass::nonConstGetter,
                      &PrototypeTestClass::setReturnString);
    REQUIRE(property.get(&testClass) == "foobar");
    property.set(&testClass, "baz");
    REQUIRE(property.get(&testClass) == "baz");
  }

  SECTION("name") {
    Property property("prop", &PrototypeTestClass::strReturnNoArgs);
    REQUIRE(property.name() == "prop");
  }
}