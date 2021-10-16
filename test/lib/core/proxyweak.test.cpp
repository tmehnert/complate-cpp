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
#include <complate/core/proxyweak.h>

#include "catch2/catch.hpp"

using namespace complate;
using namespace std;
using Catch::Matchers::Equals;

class ProxyWeakTestClass {};

TEST_CASE("ProxyWeak", "[core]") {
  ProxyWeakTestClass proxyWeakTestClass;

  SECTION("traits") {
    REQUIRE(is_copy_constructible_v<ProxyWeak>);
    REQUIRE(is_copy_assignable_v<ProxyWeak>);
    REQUIRE(is_move_constructible_v<ProxyWeak>);
    REQUIRE(is_move_assignable_v<ProxyWeak>);
  }

  SECTION("constructed without name, take typeid for name") {
    auto proxy = ProxyWeak(&proxyWeakTestClass);
    REQUIRE_THAT(proxy.name(), Equals(typeid(ProxyWeakTestClass).name()));
  }

  SECTION("constructed with name, take name") {
    auto proxy = ProxyWeak("MyProxyWeakTestClass", &proxyWeakTestClass);
    REQUIRE_THAT(proxy.name(), Equals("MyProxyWeakTestClass"));
  }

  SECTION("ptr return the object") {
    auto proxy = ProxyWeak("ProxyWeakTestClass", &proxyWeakTestClass);
    REQUIRE(proxy.ptr() == &proxyWeakTestClass);
  }

  SECTION("operator==/!=") {
    auto proxy = ProxyWeak("ProxyWeakTestClass", &proxyWeakTestClass);

    SECTION("this is equal") { REQUIRE(proxy == proxy); }

    SECTION("with same values is equal") {
      auto another = ProxyWeak("ProxyWeakTestClass", &proxyWeakTestClass);
      REQUIRE(another == proxy);
    }

    SECTION("with different name is not equal") {
      auto another = ProxyWeak("AnotherName", &proxyWeakTestClass);
      REQUIRE(another != proxy);
    }

    SECTION("with different object instance is not equal") {
      ProxyWeakTestClass antoherInstance;
      auto another = ProxyWeak("ProxyWeakTestClass", &antoherInstance);
      REQUIRE(another != proxy);
    }
  }
}