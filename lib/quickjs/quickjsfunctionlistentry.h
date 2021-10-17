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
#pragma once

#include "quickjs.h"

class QuickJsFunctionListEntry {
public:
  QuickJsFunctionListEntry() = delete;

  static constexpr JSCFunctionListEntry cfunc(const char *name, uint8_t length,
                                              JSCFunction *func) {
    JSCFunctionListEntry e{};
    e.name = name;
    e.prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
    e.def_type = JS_DEF_CFUNC;
    e.magic = 0;
    e.u.func.length = length;
    e.u.func.cproto = JS_CFUNC_generic;
    e.u.func.cfunc.generic = func;
    return e;
  }
};