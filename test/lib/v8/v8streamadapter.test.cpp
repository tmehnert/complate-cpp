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
#include "../../../lib/v8/v8streamadapter.h"

#include <v8.h>

#include "catch2/catch.hpp"
#include "core/stream.mock.h"

using namespace trompeloeil;
using namespace complate;
using namespace std;

TEST_CASE("V8StreamAdapter", "[v8]") {
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate *isolate = v8::Isolate::New(params);
  v8::Locker locker(isolate);
  v8::HandleScope hscope(isolate);
  auto context = v8::Context::New(isolate);
  v8::Context::Scope cscope(context);

  auto stream = StreamMock();
  V8StreamAdapter streamAdapter(isolate);
  string source =
      "var testWriteLong = function(stream) {\n"
      "  stream.write('" +
      string(2048, '*') +
      "');\n"
      "}\n";
  source += R"(
      var testWrite = function(stream) {
        stream.write('7 chars');
      }
      var testWriteln = function(stream) {
        stream.writeln('8 chars*');
      }
      var testFlush = function(stream) {
        stream.flush();
      }
  )";

  v8::Local<v8::String> src =
      v8::String::NewFromUtf8(isolate, source.c_str(),
                              v8::NewStringType::kNormal)
          .ToLocalChecked();
  v8::Local<v8::Script> script =
      v8::Script::Compile(context, src).ToLocalChecked();
  script->Run(context).ToLocalChecked();

  v8::Local<v8::Object> global = context->Global();

  SECTION("forward write to stream") {
    const string expectedStr = "7 chars";
    REQUIRE_CALL(stream, write(eq<const char *>(expectedStr), eq<int>(7)))
        .TIMES(1);

    v8::Local<v8::Function> value =
        global
            ->Get(context, v8::String::NewFromUtf8(isolate, "testWrite",
                                                   v8::NewStringType::kNormal)
                               .ToLocalChecked())
            .ToLocalChecked()
            .As<v8::Function>();
    v8::Local<v8::Value> args[1];
    args[0] = streamAdapter.adapterFor(stream);
    value->Call(context, global, 1, args).ToLocalChecked();
  }

  SECTION("forward write to stream (longpath)") {
    const string expectedStr(2048, '*');
    REQUIRE_CALL(stream, write(eq<const char *>(expectedStr), eq<int>(2048)))
        .TIMES(1);

    v8::Local<v8::Function> value =
        global
            ->Get(context, v8::String::NewFromUtf8(isolate, "testWriteLong",
                                                   v8::NewStringType::kNormal)
                               .ToLocalChecked())
            .ToLocalChecked()
            .As<v8::Function>();
    v8::Local<v8::Value> args[1];
    args[0] = streamAdapter.adapterFor(stream);
    value->Call(context, global, 1, args).ToLocalChecked();
  }

  SECTION("forward writeln to stream") {
    const string expectedStr = "8 chars*";
    REQUIRE_CALL(stream, writeln(eq<const char *>(expectedStr), eq<int>(8)))
        .TIMES(1);

    v8::Local<v8::Function> value =
        global
            ->Get(context, v8::String::NewFromUtf8(isolate, "testWriteln",
                                                   v8::NewStringType::kNormal)
                               .ToLocalChecked())
            .ToLocalChecked()
            .As<v8::Function>();
    v8::Local<v8::Value> args[1];
    args[0] = streamAdapter.adapterFor(stream);
    value->Call(context, global, 1, args).ToLocalChecked();
  }

  SECTION("forward flush to stream") {
    REQUIRE_CALL(stream, flush()).TIMES(1);

    v8::Local<v8::Function> value =
        global
            ->Get(context, v8::String::NewFromUtf8(isolate, "testFlush",
                                                   v8::NewStringType::kNormal)
                               .ToLocalChecked())
            .ToLocalChecked()
            .As<v8::Function>();
    v8::Local<v8::Value> args[1];
    args[0] = streamAdapter.adapterFor(stream);
    value->Call(context, global, 1, args).ToLocalChecked();
  }
}
