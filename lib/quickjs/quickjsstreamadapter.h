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

#include <complate/core/stream.h>

#include <array>
#include <memory>

#include "quickjs.h"
#include "quickjsfunctionlistentry.h"

namespace complate {

class QuickJsStreamAdapter {
public:
  explicit QuickJsStreamAdapter(JSContext *context);

  JSValue adapterFor(Stream &stream);

private:
  JSContext *m_context;
  static JSClassID ms_class_id;

  static void registerClass(JSContext *context);

  static JSValue write(JSContext *ctx, JSValueConst this_val, int argc,
                       JSValueConst *argv);
  static JSValue writeln(JSContext *ctx, JSValueConst this_val, int argc,
                         JSValueConst *argv);
  static JSValue flush(JSContext *ctx, JSValueConst this_val, int argc,
                       JSValueConst *argv);

  static constexpr std::array<JSCFunctionListEntry, 3> MSCE_FUNCTIONS{
      QuickJsFunctionListEntry::cfunc("write", 1, write),
      QuickJsFunctionListEntry::cfunc("writeln", 1, writeln),
      QuickJsFunctionListEntry::cfunc("flush", 0, flush)};
};
}  // namespace complate
