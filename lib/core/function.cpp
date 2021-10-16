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
#include <complate/core/function.h>
#include <complate/core/value.h>

using namespace complate;
using namespace std;

Function::Function() : m_callback(undefined) {}

Function::Function(Callback callback) : m_callback(move(callback)) {}

Function::Function(const CallbackWithoutArgs &callback) {
  m_callback = [callback](const Array &) { return callback(); };
}

Function Function::noop() { return {}; }

Value Function::apply(const Array &args) const { return m_callback(args); }

bool Function::operator==(const Function &) const { return true; }

bool Function::operator!=(const Function &) const { return false; }

Value Function::undefined(const Array &) { return {}; }
