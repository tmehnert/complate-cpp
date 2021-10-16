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
#include "quickjshelper.h"

#include <complate/core/exception.h>

using namespace complate;
using namespace std;

void QuickJsHelper::evaluateSource(JSContext *context, const string &source) {
  JSValue rc = JS_Eval(context, source.c_str(), source.size(), "<source>",
                       JS_EVAL_TYPE_GLOBAL);
  if (JS_IsException(rc)) {
    throw Exception(JS_ToCString(context, JS_GetException(context)));
  }
  JS_FreeValue(context, rc);
}

JSValue QuickJsHelper::getFunction(JSContext *context, const string &name) {
  JSValue func = JS_Eval(context, name.c_str(), name.size(), "<func>",
                         JS_EVAL_TYPE_GLOBAL);
  if (JS_IsException(func)) {
    JS_FreeValue(context, func);
    JSValue exc = JS_GetException(context);
    const char *str = JS_ToCString(context, exc);
    JS_FreeValue(context, exc);
    throw Exception(str);
  } else if (!JS_IsFunction(context, func)) {
    JS_FreeValue(context, func);
    throw Exception("Is not a function");
  }

  return func;
}
