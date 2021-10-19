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
#include "../../lib/v8/v8mapper.h"

#include "catch2/catch.hpp"
#include "testdata.h"

using namespace Catch::Matchers;
using namespace Catch::literals;
using namespace complate;
using namespace std;

TEST_CASE("V8Mapper", "[v8]") {
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate *isolate = v8::Isolate::New(params);
  v8::Locker locker(isolate);
  v8::HandleScope hscope(isolate);
  auto context = v8::Context::New(isolate);
  v8::Context::Scope cscope(context);
  V8Mapper mapper(isolate, Testdata::prototypes());

  auto v8String = [isolate](const string &str) -> v8::Local<v8::String> {
    return v8::String::NewFromUtf8(isolate, str.c_str(),
                                   v8::NewStringType::kNormal)
        .ToLocalChecked();
  };

  SECTION("fromObject") {
    SECTION("convert properties") {
      const Object obj = {
          {"name", "Klaus"}, {"age", 23}, {"lucky", true}, {"car", nullptr}};
      v8::Local<v8::Object> objh = mapper.fromObject(obj);
      REQUIRE(
          objh->Get(context, v8String("name")).ToLocalChecked()->IsString());
      REQUIRE(objh->Get(context, v8String("age")).ToLocalChecked()->IsInt32());
      REQUIRE(
          objh->Get(context, v8String("lucky")).ToLocalChecked()->IsBoolean());
      REQUIRE(objh->Get(context, v8String("car")).ToLocalChecked()->IsNull());
    }

    SECTION("convert nested arrays") {
      const Object obj = {{"items", Array{"Eating", -23, true, nullptr}}};
      v8::Local<v8::Object> objh = mapper.fromObject(obj);
      v8::Local<v8::Array> arr = objh->Get(context, v8String("items"))
                                     .ToLocalChecked()
                                     .As<v8::Array>();
      REQUIRE(arr->Get(context, 0).ToLocalChecked()->IsString());
      REQUIRE(arr->Get(context, 1).ToLocalChecked()->IsInt32());
      REQUIRE(arr->Get(context, 2).ToLocalChecked()->IsBoolean());
      REQUIRE(arr->Get(context, 3).ToLocalChecked()->IsNull());
    }

    SECTION("convert nested objects") {
      const Object obj = {
          {"address", Object{{"street", "Hamburger Str."}, {"houseno", 155}}}};
      v8::Local<v8::Object> objh = mapper.fromObject(obj);
      v8::Local<v8::Object> address = objh->Get(context, v8String("address"))
                                          .ToLocalChecked()
                                          .As<v8::Object>();
      REQUIRE(address->Get(context, v8String("street"))
                  .ToLocalChecked()
                  ->IsString());
      REQUIRE(address->Get(context, v8String("houseno"))
                  .ToLocalChecked()
                  ->IsInt32());
    }
  }

  SECTION("fromArray") {
    SECTION("convert nested objects") {
      const Array arr = {Object{{"name", "John"}}, Object{{"name", "Jane"}}};

      v8::Local<v8::Array> d = mapper.fromArray(arr);
      REQUIRE(d->IsArray());
      auto john = d->Get(context, 0)
                      .ToLocalChecked()
                      ->ToObject(context)
                      .ToLocalChecked();
      REQUIRE(john->IsObject());
      v8::String::Utf8Value nameOfJohn(isolate,
                                       john->Get(context, v8String("name"))
                                           .ToLocalChecked()
                                           ->ToString(context)
                                           .ToLocalChecked());
      REQUIRE_THAT(*nameOfJohn, Equals("John"));

      auto jane = d->Get(context, 1)
                      .ToLocalChecked()
                      ->ToObject(context)
                      .ToLocalChecked();
      REQUIRE(jane->IsObject());
      v8::String::Utf8Value nameOfJane(isolate,
                                       jane->Get(context, v8String("name"))
                                           .ToLocalChecked()
                                           ->ToString(context)
                                           .ToLocalChecked());
      REQUIRE_THAT(*nameOfJane, Equals("Jane"));
    }
  }

  SECTION("fromValue") {
    SECTION("convert undefined") {
      v8::Local<v8::Value> value = mapper.fromValue(Value());
      REQUIRE(value->IsUndefined());
    }

    SECTION("convert null") {
      v8::Local<v8::Value> value = mapper.fromValue(Null());
      REQUIRE(value->IsNull());
    }

    SECTION("convert true") {
      v8::Local<v8::Value> value = mapper.fromValue((bool)true);
      REQUIRE(value->IsBoolean());
      REQUIRE(value->ToBoolean(isolate)->Value() == true);
    }

    SECTION("convert false") {
      v8::Local<v8::Value> value = mapper.fromValue((bool)false);
      REQUIRE(value->IsBoolean());
      REQUIRE(value->ToBoolean(isolate)->Value() == false);
    }

    SECTION("convert int32") {
      v8::Local<v8::Value> value = mapper.fromValue((int32_t)-23);
      REQUIRE(value->IsInt32());
      REQUIRE(value->ToInt32(context).ToLocalChecked()->Value() == -23);
    }

    SECTION("convert uint32") {
      v8::Local<v8::Value> value = mapper.fromValue((uint32_t)42);
      REQUIRE(value->IsUint32());
      REQUIRE(value->ToUint32(context).ToLocalChecked()->Value() ==
              (uint32_t)42);
    }

    SECTION("convert int64") {
      v8::Local<v8::Value> value = mapper.fromValue((int64_t)-112344567879);
      REQUIRE(value->IsBigInt());
      REQUIRE(value->ToBigInt(context).ToLocalChecked()->Int64Value() ==
              (int64_t)-112344567879);
    }

    SECTION("convert double") {
      v8::Local<v8::Value> value = mapper.fromValue((double)3.14);
      REQUIRE(value->IsNumber());
      REQUIRE(value->ToNumber(context).ToLocalChecked()->Value() == 3.14_a);
    }

    SECTION("convert const char* nullptr") {
      const char *str = nullptr;
      v8::Local<v8::Value> value = mapper.fromValue(str);
      REQUIRE(value->IsNull());
    }

    SECTION("convert String") {
      v8::Local<v8::Value> value = mapper.fromValue(String("Hello string!"));
      REQUIRE(value->IsString());
      v8::String::Utf8Value str(isolate,
                                value->ToString(context).ToLocalChecked());
      REQUIRE_THAT(*str, Equals("Hello string!"));
    }

    SECTION("convert Array") {
      v8::Local<v8::Value> value =
          mapper.fromValue(Array{0, "foo", nullptr, false});
      REQUIRE(value->IsArray());
      auto arr = v8::Array::Cast(*value);
      REQUIRE(arr->Get(context, 0).ToLocalChecked()->IsInt32());
      REQUIRE(arr->Get(context, 1).ToLocalChecked()->IsString());
      REQUIRE(arr->Get(context, 2).ToLocalChecked()->IsNull());
      REQUIRE(arr->Get(context, 3).ToLocalChecked()->IsBoolean());
    }

    SECTION("convert Object") {
      v8::Local<v8::Value> value =
          mapper.fromValue(Object{{"a", 16}, {"b", "hi"}});
      REQUIRE(value->IsObject());
      auto obj = v8::Object::Cast(*value);
      REQUIRE(obj->Get(context, v8String("a")).ToLocalChecked()->IsInt32());
      REQUIRE(obj->Get(context, v8String("b")).ToLocalChecked()->IsString());
    }

    SECTION("convert Function") {
      Value f = Function{[](const Array &args) { return args.at(1); }};
      v8::Local<v8::Value> value = mapper.fromValue(f);
      REQUIRE(value->IsFunction());
      v8::Function *fn = v8::Function::Cast(*value);
      v8::Local<v8::Value> args[2];
      args[0] = mapper.fromValue(1);
      args[1] = mapper.fromValue(2);
      v8::Local<v8::Value> res =
          fn->Call(context, context->Global(), 2, args).ToLocalChecked();
      REQUIRE(res->IsInt32());
      REQUIRE(res->ToInt32(context).ToLocalChecked()->Value() == 2);
    }

    SECTION("convert Proxy") {
      v8::Local<v8::Value> value =
          mapper.fromValue(Proxy{"std::string", make_shared<string>()});
      REQUIRE(value->IsObject());
      auto object = value->ToObject(context).ToLocalChecked();
      auto method = object->Get(
          context,
          v8::String::NewFromUtf8(isolate, "clear", v8::NewStringType::kNormal)
              .ToLocalChecked());
      REQUIRE(method.ToLocalChecked()->IsFunction());
    }

    SECTION("convert ProxyWeak") {
      string foo = "foo";
      v8::Local<v8::Value> value =
          mapper.fromValue(ProxyWeak{"std::string", &foo});
      REQUIRE(value->IsObject());
      auto object = value->ToObject(context).ToLocalChecked();
      auto method = object->Get(
          context,
          v8::String::NewFromUtf8(isolate, "clear", v8::NewStringType::kNormal)
              .ToLocalChecked());
      REQUIRE(method.ToLocalChecked()->IsFunction());
    }
  }
}
