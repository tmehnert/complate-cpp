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
#include "v8streamadapter.h"

using namespace complate;

V8StreamAdapter::V8StreamAdapter(v8::Isolate *isolate) : m_isolate(isolate) {
  v8::Locker locker(m_isolate);
  v8::HandleScope scope(m_isolate);
  auto tmpl = v8::ObjectTemplate::New(m_isolate);
  tmpl->SetInternalFieldCount(1);

  tmpl->Set(m_isolate, "write", v8::FunctionTemplate::New(m_isolate, write));
  tmpl->Set(m_isolate, "writeln",
            v8::FunctionTemplate::New(m_isolate, writeln));
  tmpl->Set(m_isolate, "flush", v8::FunctionTemplate::New(m_isolate, flush));
  m_template.Reset(m_isolate, tmpl);
}

v8::Local<v8::Object> V8StreamAdapter::adapterFor(Stream &stream) {
  auto adapter = m_template.Get(m_isolate)
                     ->NewInstance(m_isolate->GetCurrentContext())
                     .ToLocalChecked();
  adapter->SetInternalField(0, v8::External::New(m_isolate, &stream));
  return adapter;
}

void V8StreamAdapter::write(const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::Local<v8::String> str =
      args[0]
          ->ToString(args.GetIsolate()->GetCurrentContext())
          .ToLocalChecked();
  char buf[FAST_UTF8_BUFLEN];
  int chars;
  int len = str->WriteUtf8(args.GetIsolate(), buf, FAST_UTF8_BUFLEN - 1, &chars,
                           v8::String::NO_NULL_TERMINATION);
  buf[len] = '\0';
  if (str->Length() == chars) {
    stream(args)->write(buf, len);
  } else {
    v8::String::Utf8Value utf8str(args.GetIsolate(), str);
    stream(args)->write(*utf8str, utf8str.length());
  }
}

void V8StreamAdapter::writeln(const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::String::Utf8Value str(args.GetIsolate(), args[0]);
  stream(args)->writeln(*str, str.length());
}

void V8StreamAdapter::flush(const v8::FunctionCallbackInfo<v8::Value> &args) {
  stream(args)->flush();
}

Stream *V8StreamAdapter::stream(
    const v8::FunctionCallbackInfo<v8::Value> &args) {
  auto intern =
      v8::Local<v8::External>::Cast(args.This()->GetInternalField(0))->Value();
  auto stream = static_cast<Stream *>(intern);
  return stream;
}
