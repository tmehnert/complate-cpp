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
#include <complate/core/prototype.h>

#include "catch2/catch.hpp"
#include "prototypetestclass.h"

using namespace complate;
using namespace std;

TEST_CASE("Prototype", "[core]") {
  SECTION("constructed with name") {
    auto prototype = Prototype("PrototypeTestClass");
    REQUIRE(prototype.name() == "PrototypeTestClass");
  }

  SECTION("constructed by copy") {
    Prototype prototype1("PrototypeTestClass");
    prototype1.addMethod(
        {"noReturnWithArgs", &PrototypeTestClass::noReturnWithArgs});
    prototype1.addProperty(
        {"strReturnNoArgs", &PrototypeTestClass::strReturnNoArgs});
    Prototype prototype2 = prototype1;  // NOLINT - I need copy for testing
    REQUIRE(prototype1.name() == prototype2.name());
    REQUIRE(prototype1.methods().size() == prototype2.methods().size());
    REQUIRE(prototype1.properties().size() == prototype2.properties().size());
  }

  SECTION("add a method") {
    Prototype prototype("PrototypeTestClass");
    prototype.addMethod(
        {"noReturnWithArgs", &PrototypeTestClass::noReturnWithArgs});

    Method method = prototype.methods().front();
    REQUIRE(method.name() == "noReturnWithArgs");
  }

  SECTION("get a method") {
    Prototype prototype("PrototypeTestClass");
    prototype.addMethod(
        {"noReturnWithArgs", &PrototypeTestClass::noReturnWithArgs});

    optional<Method> method = prototype.method("noReturnWithArgs");
    REQUIRE(method.has_value());
    REQUIRE(method.value().name() == "noReturnWithArgs");
    REQUIRE_FALSE(prototype.method("thisMethodNotExist").has_value());
  }

  SECTION("add a property") {
    Prototype prototype("PrototypeTestClass");
    prototype.addProperty(
        {"strReturnNoArgs", &PrototypeTestClass::strReturnNoArgs});

    Property property = prototype.properties().front();
    REQUIRE(property.name() == "strReturnNoArgs");
  }

  SECTION("get a property") {
    Prototype prototype("PrototypeTestClass");
    prototype.addProperty(
        {"strReturnNoArgs", &PrototypeTestClass::strReturnNoArgs});

    optional<Property> property = prototype.property("strReturnNoArgs");
    REQUIRE(property.has_value());
    REQUIRE(property.value().name() == "strReturnNoArgs");
    REQUIRE_FALSE(prototype.property("thisPropertyNotExist").has_value());
  }
}