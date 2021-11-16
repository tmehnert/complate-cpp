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
#include "../../lib/v8/v8renderercontext.h"

#include <complate/v8/v8platform.h>

#include "catch2/catch.hpp"

using namespace complate;
using namespace std;

TEST_CASE("V8Unmapper", "[v8]") {
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate *isolate = v8::Isolate::New(params);
  v8::Locker locker(isolate);
  v8::HandleScope hscope(isolate);
  auto context = v8::Context::New(isolate);
  v8::Context::Scope cscope(context);
  V8RendererContext rctx(isolate);
  auto &mapper = rctx.mapper();
  auto &unmapper = rctx.unmapper();

  SECTION("fromValue") {
    SECTION("unmap undefined") {
      auto value = mapper.fromValue(Value{});
      const Value unmapped = unmapper.fromValue(value);
      REQUIRE(unmapped.holds<Undefined>());
    }

    SECTION("unmap null") {
      auto value = mapper.fromValue(nullptr);
      const Value unmapped = unmapper.fromValue(value);
      REQUIRE(unmapped.holds<Null>());
    }

    SECTION("unmap true") {
      auto value = mapper.fromValue(true);
      const Value unmapped = unmapper.fromValue(value);
      REQUIRE(unmapped.holds<Bool>());
      REQUIRE(unmapped.exactly<Bool>() == true);
    }

    SECTION("unmap false") {
      auto value = mapper.fromValue(false);
      const Value unmapped = unmapper.fromValue(value);
      REQUIRE(unmapped.holds<Bool>());
      REQUIRE(unmapped.exactly<Bool>() == false);
    }

    SECTION("unmap int32_t") {
      auto value = mapper.fromValue((int32_t)-32);
      const Value unmapped = unmapper.fromValue(value);
      const Number number = unmapped.exactly<Number>();
      REQUIRE(number.holds<int32_t>());
      REQUIRE(number.exactly<int32_t>() == -32);
    }

    SECTION("unmap uint32_t") {
      auto value = mapper.fromValue((uint32_t)815);
      const Value unmapped = unmapper.fromValue(value);
      const Number number = unmapped.exactly<Number>();
      REQUIRE(number.holds<uint32_t>());
      REQUIRE(number.exactly<uint32_t>() == 815);
    }

    SECTION("unmap int64_t") {
      auto value = mapper.fromValue((int64_t)-123000);
      const Value unmapped = unmapper.fromValue(value);
      const Number number = unmapped.exactly<Number>();
      REQUIRE(number.holds<int64_t>());
      REQUIRE(number.exactly<int64_t>() == -123000);
    }

    SECTION("unmap double") {
      auto value = mapper.fromValue(3.1415);
      const Value unmapped = unmapper.fromValue(value);
      const Number number = unmapped.exactly<Number>();
      REQUIRE(number.holds<double>());
      REQUIRE(number.exactly<double>() == Approx(3.1415));
    }

    SECTION("unmap string") {
      auto value = mapper.fromValue("Hello World!");
      const Value unmapped = unmapper.fromValue(value);
      REQUIRE(unmapped.holds<String>());
      REQUIRE(unmapped.exactly<String>() == "Hello World!");
    }

    SECTION("unmap array") {
      auto value = mapper.fromValue(Array{1, true, "foo"});
      const Value unmapped = unmapper.fromValue(value);
      REQUIRE(unmapped.holds<Array>());
      REQUIRE(unmapped.exactly<Array>() == Array{1, true, "foo"});
    }
  }

  SECTION("fromArray") {
    SECTION("unmap empty") {
      auto value = mapper.fromValue(Array{});
      const Value unmapped = unmapper.fromValue(value);
      REQUIRE(unmapped.holds<Array>());
      REQUIRE(unmapped.exactly<Array>().empty());
    }
  }
}
