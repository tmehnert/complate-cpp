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
#include <complate/core/method.h>

#include "catch2/catch.hpp"
#include "prototypetestclass.h"

using namespace complate;
using namespace std;

TEST_CASE("Method", "[core]") {
  PrototypeTestClass testClass("foobar");

  SECTION("conststructed by copy") {
    Method method1("noReturnNoArgs", &PrototypeTestClass::noReturnNoArgs);
    Method method2 = method1;  // NOLINT - I want to copy to test copy
    REQUIRE(method1.name() == method2.name());
    method1.apply(&testClass, {});
    method2.apply(&testClass, {});
    REQUIRE(testClass.cntNoReturnNoArgsCalled() == 2);
  }

  SECTION("no return, no args") {
    Method method("noReturnNoArgs", &PrototypeTestClass::noReturnNoArgs);
    Value value = method.apply(&testClass, {});
    REQUIRE(testClass.cntNoReturnNoArgsCalled() == 1);
    REQUIRE(Value() == value);
  }

  SECTION("no return, with args") {
    Method method("noReturnWithArgs", &PrototypeTestClass::noReturnWithArgs);
    Array args = {"answer", 42};
    Value value = method.apply(&testClass, args);
    REQUIRE(testClass.cntNoReturnWithArgsCalled() == 1);
    REQUIRE(Value() == value);
    REQUIRE(testClass.args() == args);
  }

  SECTION("str return, no args") {
    Method method("strReturnNoArgs", &PrototypeTestClass::strReturnNoArgs);
    Value value = method.apply(&testClass, {});
    REQUIRE(value == "foobar");
  }

  SECTION("str return, with args") {
    Method method("strReturnWithArgs", &PrototypeTestClass::strReturnWithArgs);
    Array args = {"answer", 42};
    Value value = method.apply(&testClass, args);
    REQUIRE(testClass.cntStrReturnWithArgsCalled() == 1);
    REQUIRE(value == "foobar");
    REQUIRE(testClass.args() == args);
  }

  SECTION("name") {
    Method method("noReturnNoArgs", &PrototypeTestClass::noReturnNoArgs);
    REQUIRE(method.name() == "noReturnNoArgs");
  }
}