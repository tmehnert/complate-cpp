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
#include <string>

#include "prototype.h"

namespace complate {

/**
 * Help constructing a Prorotype for a native C++ class.
 *
 * With this builder you can easily build your class prototypes.
 *
 * @tparam T The class you want to build an Prototype for.
 *
 * @example
 * @code
 * Prototype prototype = PrototypeBuilder<MyClass>()
 *   .property("someData", &Myclass::getSomeData)
 *   .method("makeSomething", &MyClass::makeSomething)
 *   .method<string>("lambdaSomething", [] (MyClass &m, const Array &args) {
 *      return m.something(args.at(0).get<int>().value());
 *   })
 *   .build();
 */
template <typename T>
class PrototypeBuilder {
public:
  /** Construct a PrototypeBuilder with given Prototype name. */
  explicit PrototypeBuilder<T>(std::string name)
      : m_prototype(std::move(name)){};

  /**
   * Construct a PrototypeBuilder with the Prototype name taken from T.
   *
   * @note Because compilers don't have typeid(T).name() standardized,
   * you should not use this, when you create a shared library and
   * what to export your Proxy and Prototype.
   */
  PrototypeBuilder<T>() : PrototypeBuilder(typeid(T).name()){};

  /** Add a Method from a member function with arguments. */
  template <typename R>
  PrototypeBuilder<T> &method(std::string name,
                              R (T::*callback)(const Array &args)) {
    m_prototype.addMethod({name, callback});
    return *this;
  }

  /** Add a Method from a member function without arguments. */
  template <typename R>
  PrototypeBuilder<T> &method(std::string name, R (T::*callback)()) {
    m_prototype.addMethod({name, callback});
    return *this;
  }

  /** Add a const Method from a member function without arguments. */
  template <typename R>
  PrototypeBuilder<T> &method(std::string name, R (T::*callback)() const) {
    m_prototype.addMethod({name, callback});
    return *this;
  }

  /** Add a Method from a lambda with arguments. */
  template <typename R>
  PrototypeBuilder<T> &method(
      std::string name, std::function<R(T &, const Array &args)> callback) {
    m_prototype.addMethod(
        {name, [callback](void *p, const Array &args) -> Value {
           if constexpr (std::is_void_v<std::invoke_result_t<
                             decltype(callback), T &, const Array &>>) {
             std::invoke(callback, *static_cast<T *>(p), args);
             return {};
           } else {
             return std::invoke(callback, *static_cast<T *>(p), args);
           }
         }});
    return *this;
  }

  /** Add a Method from a lambda without arguments. */
  template <typename R>
  PrototypeBuilder<T> &method(std::string name,
                              std::function<R(T &)> callback) {
    m_prototype.addMethod(
        {name, [callback](void *p, const Array &) -> Value {
           if constexpr (std::is_void_v<
                             std::invoke_result_t<decltype(callback), T &>>) {
             std::invoke(callback, *static_cast<T *>(p));
             return {};
           } else {
             return std::invoke(callback, *static_cast<T *>(p));
           }
         }});
    return *this;
  }

  /** Add a readonly Property from a member function. */
  template <typename R>
  PrototypeBuilder<T> &property(std::string name, R (T::*getter)() const) {
    m_prototype.addProperty({name, getter});
    return *this;
  }

  /** Add a readonly Property from a non-const member function. */
  template <typename R>
  PrototypeBuilder<T> &property(std::string name, R (T::*getter)()) {
    m_prototype.addProperty({name, getter});
    return *this;
  }

  /** Add a writable Property from a member function. */
  template <typename R>
  PrototypeBuilder<T> &property(std::string name, R (T::*getter)() const,
                                void (T::*setter)(const Value &)) {
    m_prototype.addProperty({name, getter, setter});
    return *this;
  }

  /** Add a writable Property from a non-const member function. */
  template <typename R>
  PrototypeBuilder<T> &property(std::string name, R (T::*getter)(),
                                void (T::*setter)(const Value &)) {
    m_prototype.addProperty({name, getter, setter});
    return *this;
  }

  /** Add a readonly Property from a lambda. */
  template <typename R>
  PrototypeBuilder<T> &property(std::string name,
                                std::function<R(T &)> getter) {
    m_prototype.addProperty({name, [getter](void *p) -> Value {
                               return std::invoke(getter, *static_cast<T *>(p));
                             }});
    return *this;
  }

  /** Add a writable Property from a lambda. */
  template <typename R>
  PrototypeBuilder<T> &property(
      std::string name, std::function<R(T &)> getter,
      std::function<void(T &, const Value &)> setter) {
    m_prototype.addProperty({name,
                             [getter](void *p) -> Value {
                               return std::invoke(getter, *static_cast<T *>(p));
                             },
                             [setter](void *p, const Value &value) {
                               std::invoke(setter, *static_cast<T *>(p), value);
                             }});
    return *this;
  }

  /** Add a writable Property from member function getter and lambda setter. */
  template <typename R>
  PrototypeBuilder<T> &property(
      std::string name, R (T::*getter)() const,
      std::function<void(T &, const Value &)> setter) {
    m_prototype.addProperty({name,
                             [getter](void *p) {
                               return std::invoke(getter, *static_cast<T *>(p));
                             },
                             [setter](void *p, const Value &value) {
                               std::invoke(setter, *static_cast<T *>(p), value);
                             }});
    return *this;
  }

  /** Add a writable Property from lambda getter and member function setter. */
  template <typename R>
  PrototypeBuilder<T> &property(std::string name, std::function<R(T &)> getter,
                                void (T::*setter)(const Value &)) {
    m_prototype.addProperty({name,
                             [getter](void *p) -> Value {
                               return std::invoke(getter, *static_cast<T *>(p));
                             },
                             [setter](void *p, const Value &value) {
                               std::invoke(setter, *static_cast<T *>(p), value);
                             }});
    return *this;
  }

  /** Build the Prototype. */
  [[nodiscard]] Prototype build() { return m_prototype; }

private:
  /** Not really a builder pattern, but a easier interface */
  Prototype m_prototype;
};
}  // namespace complate
