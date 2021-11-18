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
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "../../../lib/v8/v8renderercontext.h"

#include <v8.h>

#include "catch2/catch.hpp"
#include "testdata.h"

using namespace complate;
using namespace std;

TEST_CASE("V8MapperBenchmark", "[v8][.benchmark]") {
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate *isolate = v8::Isolate::New(params);
  v8::Locker locker(isolate);
  v8::HandleScope hscope(isolate);
  auto context = v8::Context::New(isolate);
  v8::Context::Scope cscope(context);
  V8RendererContext rctx(isolate, Testdata::prototypes());
  auto &mapper = rctx.mapper();

  const Object parameters = Testdata::forMapperBenchmark();

  BENCHMARK("convert parameters") { mapper.fromObject(parameters); };
}