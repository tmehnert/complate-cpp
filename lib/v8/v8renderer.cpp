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
#include <complate/core/exception.h>
#include <complate/v8/v8renderer.h>
#include <v8.h>

#include <utility>

#include "v8helper.h"
#include "v8renderercontext.h"
#include "v8streamadapter.h"
#include "v8proxydeleter.h"

using namespace complate;
using namespace std;

class V8Renderer::Impl {
public:
  explicit Impl(const string &source, const std::vector<Prototype> &prototypes,
                Object bindings)
      : m_isolate(createIsolate()),
        m_rendererContext(m_isolate, prototypes),
        m_streamAdapter(m_isolate),
        m_bindings(move(bindings)) {
    V8ProxyDeleter proxyDeleter(m_rendererContext.proxyHolder());
    v8::Locker locker(m_isolate);
    v8::HandleScope handle_scope(m_isolate);
    m_context.Reset(m_isolate, v8::Context::New(m_isolate));

    auto ctx = context();
    v8::Context::Scope context_scope(ctx);

    m_rendererContext.mapper().fromObject(m_bindings, ctx->Global());

    v8::Local<v8::String> src = V8Helper::newString(m_isolate, source);
    v8::TryCatch tryCatch(m_isolate);
    v8::MaybeLocal<v8::Script> script = v8::Script::Compile(ctx, src);
    if (tryCatch.HasCaught()) {
      v8::String::Utf8Value msg(m_isolate, tryCatch.Message()->Get());
      throw Exception(*msg);
    }
    v8::MaybeLocal<v8::Value> rc = script.ToLocalChecked()->Run(ctx);
    (void)rc;
    if (tryCatch.HasCaught()) {
      v8::String::Utf8Value msg(m_isolate, tryCatch.Message()->Get());
      throw Exception(*msg);
    }

    v8::Local<v8::Object> global = ctx->Global();
    v8::Local<v8::String> rnd = V8Helper::newString(m_isolate, "render");
    v8::Local<v8::Value> value = global->Get(ctx, rnd).ToLocalChecked();
    if (!value->IsFunction()) {
      throw Exception("ReferenceError: 'render' is not defined");
    }
    m_render.Reset(m_isolate, v8::Local<v8::Function>::Cast(value));
  }

  ~Impl() { m_isolate->Dispose(); }

  void render(const string &view, const Object &parameters, Stream &stream) {
    V8ProxyDeleter proxyDeleter(m_rendererContext.proxyHolder());
    v8::Locker locker(m_isolate);
    v8::HandleScope handle_scope(m_isolate);
    auto ctx = context();
    v8::Context::Scope context_scope(ctx);

    render(view, m_rendererContext.mapper().fromObject(parameters), stream);
  }

  void render(const string &view, const string &parameters, Stream &stream) {
    v8::Locker locker(m_isolate);
    v8::HandleScope handle_scope(m_isolate);
    auto ctx = context();
    v8::Context::Scope context_scope(ctx);

    v8::Local<v8::Value> p;
    if (!v8::JSON::Parse(ctx, V8Helper::newString(m_isolate, parameters))
             .ToLocal(&p)) {
      throw Exception("SyntaxError: 'parameters' is not an object");
    }

    render(view, p, stream);
  }

private:
  v8::Isolate *m_isolate;
  v8::Persistent<v8::Function> m_render;
  v8::Persistent<v8::Context> m_context;
  V8RendererContext m_rendererContext;
  V8StreamAdapter m_streamAdapter;
  Object m_bindings;

  v8::Local<v8::Context> context() { return m_context.Get(m_isolate); }

  static v8::Isolate *createIsolate() {
    v8::Isolate::CreateParams params;
    params.array_buffer_allocator =
        v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    return v8::Isolate::New(params);
  }

  void render(const string &view, const v8::Local<v8::Value> &parameters,
              Stream &stream) {
    auto ctx = context();

    v8::Local<v8::Value> args[3];
    args[0] = V8Helper::newString(m_isolate, view);
    args[1] = parameters;
    args[2] = m_streamAdapter.adapterFor(stream);

    v8::TryCatch tryCatch(m_isolate);
    v8::MaybeLocal<v8::Value> result =
        m_render.Get(m_isolate)->Call(ctx, ctx->Global(), 3, args);
    if (result.IsEmpty() || tryCatch.HasCaught()) {
      v8::String::Utf8Value msg(m_isolate, tryCatch.Message()->Get());
      throw Exception(*msg);
    }
  }
};

V8Renderer::V8Renderer(const string &source)
    : V8Renderer(source, {}, {}) {}

V8Renderer::V8Renderer(const string &source,
                       const std::vector<Prototype> &prototypes,
                       Object bindings)
    : m_impl(make_unique<Impl>(source, prototypes, move(bindings))) {}

V8Renderer::~V8Renderer() = default;

void V8Renderer::render(const string &view, const Object &parameters,
                        Stream &stream) {
  m_impl->render(view, parameters, stream);
}

void V8Renderer::render(const string &view, const string &parameters,
                        Stream &stream) {
  m_impl->render(view, parameters, stream);
}
