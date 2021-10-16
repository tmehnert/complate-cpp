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
#include <complate/core/proxy.h>

#include "catch2/catch.hpp"

using namespace complate;
using namespace std;
using Catch::Matchers::Equals;

class ProxyTestClass {};

TEST_CASE("Proxy", "[core]") {
  SECTION("traits") {
    REQUIRE(is_copy_constructible_v<Proxy>);
    REQUIRE(is_copy_assignable_v<Proxy>);
    REQUIRE(is_move_constructible_v<Proxy>);
    REQUIRE(is_move_assignable_v<Proxy>);
  }

  SECTION("constructed without name, take typeid for name") {
    auto proxy = Proxy(make_shared<ProxyTestClass>());
    REQUIRE_THAT(proxy.name(), Equals(typeid(ProxyTestClass).name()));
  }

  SECTION("constructed with name, take name") {
    auto proxy = Proxy("MyProxyTestClass", make_shared<ProxyTestClass>());
    REQUIRE_THAT(proxy.name(), Equals("MyProxyTestClass"));
  }

  SECTION("ptr return the object") {
    auto object = make_shared<ProxyTestClass>();
    auto proxy = Proxy("ProxyTestClass", object);
    REQUIRE(proxy.ptr() == object);
  }

  SECTION("operator==/!=") {
    auto object = make_shared<ProxyTestClass>();
    auto proxy = Proxy("ProxyTestClass", object);

    SECTION("this is equal") { REQUIRE(proxy == proxy); }

    SECTION("with same values is equal") {
      auto another = Proxy("ProxyTestClass", object);
      REQUIRE(another == proxy);
    }

    SECTION("with different name is not equal") {
      auto another = Proxy("AnotherName", object);
      REQUIRE(another != proxy);
    }

    SECTION("with different object instance is not equal") {
      auto another = Proxy("ProxyTestClass", make_shared<ProxyTestClass>());
      REQUIRE(another != proxy);
    }
  }
}