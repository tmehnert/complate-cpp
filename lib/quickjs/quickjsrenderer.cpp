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
#include <complate/quickjs/quickjsrenderer.h>

#include <mutex>
#include <utility>

#include "quickjsconsole.h"
#include "quickjshelper.h"
#include "quickjsrenderercontext.h"
#include "quickjsstreamadapter.h"
#include "quickjsproxydeleter.h"

using namespace complate;
using namespace std;

class QuickJsRenderer::Impl {
public:
  explicit Impl(const string &source, const std::vector<Prototype> &prototypes,
                Object bindings)
      : m_runtime(JS_NewRuntime()),
        m_context(JS_NewContext(m_runtime)),
        m_rendererContext(m_context, prototypes),
        m_global(JS_GetGlobalObject(m_context)),
        m_render(evaluateSource(m_context, source)),
        m_streamAdapter(m_context),
        m_bindings(move(bindings)) {
    QuickJsProxyDeleter deleter(m_rendererContext.proxyHolder());
    JS_SetMaxStackSize(m_runtime, NO_STACK_LIMIT);
    ensureConsoleDefined(m_bindings);
    m_rendererContext.mapper().fromObject(m_bindings, m_global);
  }

  ~Impl() {
    JS_FreeValue(m_context, m_render);
    JS_FreeValue(m_context, m_global);
    JS_FreeContext(m_context);
    JS_FreeRuntime(m_runtime);
  }

  void render(const string &view, const Object &parameters, Stream &stream) {
    lock_guard<mutex> guard(m_mutex);
    JS_UpdateStackTop(m_runtime);
    QuickJsProxyDeleter deleter(m_rendererContext.proxyHolder());
    render(view, m_rendererContext.mapper().fromObject(parameters), stream);
  }

  void render(const string &view, const string &parameters, Stream &stream) {
    lock_guard<mutex> guard(m_mutex);
    JS_UpdateStackTop(m_runtime);
    JSValue json = JS_ParseJSON(m_context, parameters.c_str(),
                                parameters.size(), "<json>");
    if (!JS_IsObject(json)) {
      JS_FreeValue(m_context, json);
      throw Exception("SyntaxError: 'parameters' is not an object");
    }

    render(view, json, stream);
  }

private:
  static const size_t NO_STACK_LIMIT = 0;
  mutex m_mutex;
  JSRuntime *m_runtime;
  JSContext *m_context;
  QuickJsRendererContext m_rendererContext;
  JSValue m_global;
  JSValue m_render;
  QuickJsStreamAdapter m_streamAdapter;
  Object m_bindings;

  void render(const string &view, JSValue parameters, Stream &stream) {
    JSValue argv[3];
    argv[0] = JS_NewStringLen(m_context, view.c_str(), view.length());
    argv[1] = parameters;
    argv[2] = m_streamAdapter.adapterFor(stream);

    JSValue result = JS_Call(m_context, m_render, m_global, 3, argv);
    JS_FreeValue(m_context, argv[0]);
    JS_FreeValue(m_context, argv[1]);
    JS_FreeValue(m_context, argv[2]);
    JS_FreeValue(m_context, result);

    if (JS_IsException(result)) {
      JSValue exc = JS_GetException(m_context);
      const char *str = JS_ToCString(m_context, exc);
      JS_FreeValue(m_context, exc);
      throw Exception(str);
    }
  }

  static void ensureConsoleDefined(Object &obj) {
    auto it = obj.find("console");
    if (it == obj.cend()) {
      obj.emplace("console", QuickJsConsole());
    }
  }

  static JSValue evaluateSource(JSContext *context, const string &source) {
    QuickJsHelper::evaluate(context, source);
    return QuickJsHelper::getFunction(context, "render");
  }
};

QuickJsRenderer::QuickJsRenderer(const string &source)
    : QuickJsRenderer(source, {}, {}) {}

QuickJsRenderer::QuickJsRenderer(const string &source,
                                 const std::vector<Prototype> &prototypes,
                                 Object bindings)
    : m_impl(make_unique<Impl>(source, prototypes, move(bindings))) {}

QuickJsRenderer::~QuickJsRenderer() = default;

void QuickJsRenderer::render(const string &view, const Object &parameters,
                             Stream &stream) {
  m_impl->render(view, parameters, stream);
}

void QuickJsRenderer::render(const string &view, const string &parameters,
                             Stream &stream) {
  m_impl->render(view, parameters, stream);
}
