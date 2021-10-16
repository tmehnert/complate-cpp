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
#include <complate/core/prototypebuilder.h>

#include "catch2/catch.hpp"
#include "prototypetestclass.h"

using namespace complate;
using namespace std;
using Catch::Matchers::Equals;

TEST_CASE("PrototypeBuilder", "[core]") {
  PrototypeTestClass testClass("foobar");
  Prototype prototype =
      PrototypeBuilder<PrototypeTestClass>("PrototypeTestClass")
          .property("readonly", &PrototypeTestClass::strReturnNoArgs)
          .property("readonlyNonConstGetter",
                    &PrototypeTestClass::nonConstGetter)
          .property("writable", &PrototypeTestClass::strReturnNoArgs,
                    &PrototypeTestClass::setReturnString)
          .property("readonlyNonConstGetter",
                    &PrototypeTestClass::nonConstGetter,
                    &PrototypeTestClass::setReturnString)
          .property<string>(
              "readonlyLambda",
              [](const PrototypeTestClass &tc) { return tc.strReturnNoArgs(); })
          .property<string>(
              "writableLambda",
              [](const PrototypeTestClass &tc) { return tc.strReturnNoArgs(); },
              [](PrototypeTestClass &tc, const Value &value) {
                tc.setReturnString(value);
              })
          .property("writableLambdaSetter",
                    &PrototypeTestClass::strReturnNoArgs,
                    [](PrototypeTestClass &tc, const Value &value) {
                      tc.setReturnString(value);
                    })
          .property<string>(
              "writableLambdaGetter",
              [](const PrototypeTestClass &tc) { return tc.strReturnNoArgs(); },
              &PrototypeTestClass::setReturnString)
          .method("noReturnNoArgs", &PrototypeTestClass::noReturnNoArgs)
          .method("noReturnWithArgs", &PrototypeTestClass::noReturnWithArgs)
          .method("strReturnNoArgs", &PrototypeTestClass::strReturnNoArgs)
          .method("strReturnWithArgs", &PrototypeTestClass::strReturnWithArgs)
          .method<void>("lambdaNoReturnNoArgs",
                        [&](PrototypeTestClass &tc) { tc.noReturnNoArgs(); })
          .method<void>("lambdaNoReturnNoArgsConst",
                        [&](const PrototypeTestClass &) {})
          .method<string>("lambaStrReturnWithArgs",
                          [&](PrototypeTestClass &tc, const Array &args) {
                            return tc.strReturnWithArgs(args);
                          })
          .build();

  SECTION("constructed without name, take typeid for name") {
    auto proto =
        PrototypeBuilder<PrototypeTestClass>().build();
    REQUIRE_THAT(proto.name(), Equals(typeid(PrototypeTestClass).name()));
  }

  SECTION("constructed with name, take name") {
    auto proto = PrototypeBuilder<PrototypeTestClass>("MyClassName").build();
    REQUIRE_THAT(proto.name(), Equals("MyClassName"));
  }

  SECTION("method with no return, no args") {
    REQUIRE(prototype.method("noReturnNoArgs").has_value());
  }

  SECTION("method with no return, with args") {
    REQUIRE(prototype.method("noReturnWithArgs").has_value());
  }

  SECTION("method with str return, no args") {
    REQUIRE(prototype.method("strReturnNoArgs").has_value());
  }

  SECTION("method with str return, with args") {
    REQUIRE(prototype.method("strReturnWithArgs").has_value());
  }

  SECTION("method of lambda, no return, no args") {
    optional<Method> method = prototype.method("lambdaNoReturnNoArgs");
    REQUIRE(method.has_value());
    method->apply(&testClass, {});
    REQUIRE(testClass.cntNoReturnNoArgsCalled() == 1);
  }

  SECTION("method of lambda, str return, with args") {
    optional<Method> method = prototype.method("lambaStrReturnWithArgs");
    REQUIRE(method.has_value());
    Array args = {"answer", 42};
    method->apply(&testClass, args);
    REQUIRE(testClass.cntStrReturnWithArgsCalled() == 1);
    REQUIRE(testClass.args() == args);
  }

  SECTION("property, readonly") {
    optional<Property> property = prototype.property("readonly");
    REQUIRE(property.has_value());
    REQUIRE(property->get(&testClass) == "foobar");
  }

  SECTION("property, writable") {
    optional<Property> property = prototype.property("writable");
    REQUIRE(property.has_value());
    REQUIRE(property->get(&testClass) == "foobar");
    property->set(&testClass, "baz");
    REQUIRE(property->get(&testClass) == "baz");
  }

  SECTION("property, readonly lambda") {
    optional<Property> property = prototype.property("readonlyLambda");
    REQUIRE(property.has_value());
    REQUIRE(property->get(&testClass) == "foobar");
  }

  SECTION("property, writable lambda") {
    optional<Property> property = prototype.property("writableLambda");
    REQUIRE(property.has_value());
    REQUIRE(property->get(&testClass) == "foobar");
    property->set(&testClass, "baz");
    REQUIRE(property->get(&testClass) == "baz");
  }

  SECTION("property, writable lambda setter") {
    optional<Property> property = prototype.property("writableLambdaSetter");
    REQUIRE(property.has_value());
    REQUIRE(property->get(&testClass) == "foobar");
    property->set(&testClass, "baz");
    REQUIRE(property->get(&testClass) == "baz");
  }

  SECTION("property, writable lambda getter") {
    optional<Property> property = prototype.property("writableLambdaGetter");
    REQUIRE(property.has_value());
    REQUIRE(property->get(&testClass) == "foobar");
    property->set(&testClass, "baz");
    REQUIRE(property->get(&testClass) == "baz");
  }
}