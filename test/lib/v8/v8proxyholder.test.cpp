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
#include "../../lib/v8/v8proxyholder.h"

#include "catch2/catch.hpp"

using namespace complate;
using namespace std;

TEST_CASE("V8ProxyHolder", "[v8]") {
  Proxy proxy("std::string", make_shared<string>("foo"));
  REQUIRE(proxy.ptr().use_count() == 1);

  SECTION("add copy proxy") {
    V8ProxyHolder holder;
    holder.add(proxy);
    REQUIRE(proxy.ptr().use_count() == 2);
  }

  SECTION("clear remove proxy") {
    V8ProxyHolder holder;
    holder.add(proxy);
    REQUIRE(proxy.ptr().use_count() == 2);
    holder.clear();
    REQUIRE(proxy.ptr().use_count() == 1);
  }
}