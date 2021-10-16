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
#include "quickjsunmapper.h"

using namespace complate;
using namespace std;

QuickJsUnmapper::QuickJsUnmapper(JSContext *context) : m_context(context) {}

Value QuickJsUnmapper::fromValue(JSValue value) {
  if (JS_IsString(value)) {
    size_t len;
    const char *str = JS_ToCStringLen2(m_context, &len, value, 0);
    String v = string(str, len);
    JS_FreeCString(m_context, str);
    return v;
  } else if (JS_IsNumber(value)) {
    int tag = JS_VALUE_GET_TAG(value);
    if (tag == JS_TAG_FLOAT64) {
      double v;
      JS_ToFloat64(m_context, &v, value);
      return v;
    } else if (tag == JS_TAG_INT) {
      int64_t v;
      JS_ToInt64(m_context, &v, value);
      return v;
    } else if (tag == JS_TAG_BIG_INT) {
      int64_t v;
      JS_ToBigInt64(m_context, &v, value);
      return v;
    }
  } else if (JS_IsBool(value)) {
    return JS_ToBool(m_context, value) != 0;
  } else if (JS_IsNull(value)) {
    return nullptr;
  }

  return {};
}
