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
#include <memory>
#include <string>

#include "value.h"

namespace complate {

/**
 * Method of an native C++ object to be used in Javascript.
 *
 * This class can be added to a Prototype in order to make an C++ member
 * function available inside Javascript.
 *
 * @note Use PrototypeBuilder to build your Prototype more easy.
 */
class Method {
public:
  /** Callback signature. */
  using Callback = std::function<Value(void *, const Array &args)>;

  /** Constructs a Method with a callback. */
  Method(std::string name, Callback callback);

  Method(const Method &other);

  ~Method();

  /**
   * Construct with a member function accepting an Array as arguments.
   *
   * @param name Name of your method.
   * @param callback Member function pointer, for example &YourClass::setText.
   */
  template <typename T, typename R>
  Method(std::string name, R (T::*callback)(const Array &args))
      : Method(std::move(name), wrap(callback)) {}

  /**
   * Construct with a member function accepting no arguments.
   *
   * @param name Name of your method.
   * @param callback Member function pointer, for example &YourClass::getText.
   */
  template <typename T, typename R>
  Method(std::string name, R (T::*callback)())
      : Method(std::move(name), wrap(callback)) {}

  /**
   * Construct with a const member function accepting no arguments.
   *
   * @param name Name of your method.
   * @param callback Member function pointer, for example &YourClass::getText.
   */
  template <typename T, typename R>
  Method(std::string name, R (T::*callback)() const)
      : Method(std::move(name), wrap(callback)) {}

  /** Get the name of the Method */
  [[nodiscard]] const std::string &name() const;

  /**
   * Call the Method callback.
   *
   * Returns an undefined value, if nullptr passed as object.
   * No other checks will be done, so make sure your object has the same type as
   * the member function.
   *
   * @param object The pointer to the class instance (this).
   * @param args Arguments to pass to the callback.
   * @return Value returned by the Callback.
   */
  Value apply(void *object, const Array &args) const;

private:
  class Impl;

  /** Pointer to implementation */
  std::unique_ptr<Impl> m_impl;

  /** Wraps a member function pointer with an Array as args to an function with
   * accept void* */
  template <typename T, typename R>
  static Callback wrap(R (T::*callback)(const Array &args)) {
    if constexpr (std::is_void_v<R>) {
      return [callback](void *p, const Array &args) -> Value {
        (static_cast<T *>(p)->*callback)(args);
        return {};
      };
    } else {
      return [callback](void *p, const Array &args) -> Value {
        return (static_cast<T *>(p)->*callback)(args);
      };
    }
  }

  /** Wraps a member function pointer with no args to an function with accept
   * void* */
  template <typename T, typename R>
  static Callback wrap(R (T::*callback)()) {
    if constexpr (std::is_void_v<R>) {
      return [callback](void *p, const Array &) -> Value {
        (static_cast<T *>(p)->*callback)();
        return {};
      };
    } else {
      return [callback](void *p, const Array &) -> Value {
        return (static_cast<T *>(p)->*callback)();
      };
    }
  }

  /** Wraps a const member function pointer with no args to an function with
   * accept void* */
  template <typename T, typename R>
  static Callback wrap(R (T::*callback)() const) {
    if constexpr (std::is_void_v<R>) {
      return [callback](void *p, const Array &) -> Value {
        (static_cast<T *>(p)->*callback)();
        return {};
      };
    } else {
      return [callback](void *p, const Array &) -> Value {
        return (static_cast<T *>(p)->*callback)();
      };
    }
  }
};
}  // namespace complate
