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

#include <functional>
#include <vector>

#include "types.h"

namespace complate {

/**
 * Function to call C++ Functions from JavaScript.
 */
class Function {
public:
  /** Callback signature */
  using Callback = std::function<Value(const Array &)>;
  /** Callback signature without arguments */
  using CallbackWithoutArgs = std::function<Value()>;

  /**
   * Construct a function returning an undefined Value.
   *
   * @see Function:noop
   */
  Function();

  /**
   * Construct a Function which returns an Value.
   *
   * When called from JavaScript, each argument will be an Value.
   * All the arguments will passed to your function as an Array.
   *
   * @attention Objects or Arrays as arguments itself are currently not
   * supported as function arguments from JavaScript.
   *
   * @param callback C++ function to be called.
   *
   * @example
   * @code
   * auto fn = Function{[] (const Array &args) -> Value {
   *   int arg = args.at(0).get<int>().value_or(0);
   *   return to_string(arg);
   * }};
   */
  explicit Function(Callback callback);

  /**
   * Construct a Function which returns an Value, but not any arguments.
   *
   * @param callback C++ function to be called.
   *
   * @example
   * @code
   * static std::string helloWorld() {
   *   return "Hello World!";
   * }
   * auto fn = Function{helloWorld};
   */
  explicit Function(const CallbackWithoutArgs &callback);

  /**
   * Create a function with does nothing and return an undefined Value.
   *
   * @return a function that just do nothing.
   */
  static Function noop();

  /**
   * Call the Callback
   *
   * @note Named 'apply' because clang don't like function call syntax here aka
   * 'operator()(const Array &args)'
   *
   * @param args Arguments to passed to the Callback.
   * @return Value returned by the Callback.
   */
  Value apply(const Array &args) const;  // NOLINT - Discard the result is okay

  /** Two Function always considered to be equal (always return true) */
  bool operator==(const Function &) const;

  /** Two Function always considered to not be unequal (always return false) */
  bool operator!=(const Function &) const;

private:
  Callback m_callback;

  /** Function which returns an uninitialized Value, used for default
   * constructor */
  static Value undefined(const Array &);
};
}  // namespace complate
