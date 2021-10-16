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
#include "quickjsstreamadapter.h"

#include <complate/core/exception.h>

using namespace complate;
using namespace std;

JSClassID QuickJsStreamAdapter::ms_class_id = 0;
const JSCFunctionListEntry QuickJsStreamAdapter::msc_functions[] = {
    JS_CFUNC_DEF("write", 1, write), JS_CFUNC_DEF("writeln", 1, writeln),
    JS_CFUNC_DEF("flush", 0, flush)};

QuickJsStreamAdapter::QuickJsStreamAdapter(JSContext *context)
    : m_context(context) {
  registerClass(context);
}

JSValue QuickJsStreamAdapter::adapterFor(Stream &stream) {
  JSValue v = JS_GetClassProto(m_context, ms_class_id);
  JS_SetOpaque(v, &stream);
  return v;
}

void QuickJsStreamAdapter::registerClass(JSContext *context) {
  if (ms_class_id == 0) {
    JS_NewClassID(&ms_class_id);
  }

  JSRuntime *runtime = JS_GetRuntime(context);
  if (!JS_IsRegisteredClass(runtime, ms_class_id)) {
    JSClassDef classDef{};
    classDef.class_name = "ComplateStream";
    int rc = JS_NewClass(runtime, ms_class_id, &classDef);
    if (rc < 0) {
      throw Exception("could not register JSClassDef 'ComplateStream'");
    }
  }

  JSValue proto = JS_NewObject(context);
  JS_SetPropertyFunctionList(context, proto, msc_functions, 3);
  JS_SetClassProto(context, ms_class_id, proto);
}

JSValue QuickJsStreamAdapter::write(JSContext *ctx, JSValueConst this_val, int,
                                    JSValueConst *argv) {
  auto *stream = static_cast<Stream *>(JS_GetOpaque2(ctx, this_val, 1));
  size_t len;
  const char *str = JS_ToCStringLen(ctx, &len, argv[0]);
  stream->write(str, (int)len);
  JS_FreeCString(ctx, str);
  return JS_UNDEFINED;
}

JSValue QuickJsStreamAdapter::writeln(JSContext *ctx, JSValueConst this_val,
                                      int, JSValueConst *argv) {
  auto *stream = static_cast<Stream *>(JS_GetOpaque2(ctx, this_val, 1));
  size_t len;
  const char *str = JS_ToCStringLen(ctx, &len, argv[0]);
  stream->writeln(str, (int)len);
  JS_FreeCString(ctx, str);
  return JS_UNDEFINED;
}

JSValue QuickJsStreamAdapter::flush(JSContext *ctx, JSValueConst this_val, int,
                                    JSValueConst *) {
  auto *stream = static_cast<Stream *>(JS_GetOpaque2(ctx, this_val, 1));
  stream->flush();
  return JS_UNDEFINED;
}
