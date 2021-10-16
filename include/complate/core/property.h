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

#include "value.h"

namespace complate {

/**
 * Property of an native C++ object to be used in Javascript.
 *
 * This class can be added to a Prototype in order to make Property
 * aka getter and maybe also a setter available inside Javascript.
 *
 * @note Use PrototypeBuilder to build your Prototype more easy.
 */
class Property {
public:
  /** Getter/Setter signatures. */
  using Getter = std::function<Value(void *)>;
  using Setter = std::function<void(void *, const Value &value)>;

  /** Constructs a readonly Property. */
  Property(std::string name, Getter getter);

  /** Constructs a writable Property. */
  Property(std::string name, Getter getter, Setter setter);

  Property(const Property &other);

  ~Property();

  /** Constructs a readonly Property from a member function pointer. */
  template <typename T, typename R>
  Property(std::string name, R (T::*getter)() const)
      : Property(std::move(name), wrap(getter)) {}

  /** Constructs a readonly Property from a non-const member function pointer.
   */
  template <typename T, typename R>
  Property(std::string name, R (T::*getter)())
      : Property(std::move(name), wrap(getter)) {}

  /** Constructs a writeable Property from a member function pointer. */
  template <typename T, typename R>
  Property(std::string name, R (T::*getter)() const,
           void (T::*setter)(const Value &))
      : Property(std::move(name), wrap(getter), wrap(setter)) {}

  /** Constructs a writeable Property from a non-const member function pointer.
   */
  template <typename T, typename R>
  Property(std::string name, R (T::*getter)(), void (T::*setter)(const Value &))
      : Property(std::move(name), wrap(getter), wrap(setter)) {}

  /** Get the name of the Property. */
  [[nodiscard]] const std::string &name() const;

  /**
   * Get the value of the Property.
   *
   * @param object The pointer to the class instance (this).
   * @return Value returned by the getter.
   */
  Value get(void *object) const;

  /**
   * Get the value of the Property.
   *
   * @param object The pointer to the class instance (this).
   * @param value The value passed to the setter as an argument.
   */
  void set(void *object, const Value &value) const;

private:
  class Impl;

  /** Pointer to implementation */
  std::unique_ptr<Impl> m_impl;

  /** Wraps a member function pointer getter to accept void* as this */
  template <typename T, typename R>
  static Getter wrap(R (T::*getter)() const) {
    static_assert(!std::is_void_v<R>, "A getter have to return non-void");
    return
        [getter](void *p) -> Value { return (static_cast<T *>(p)->*getter)(); };
  }

  /** Wraps a non-const member function pointer getter to accept void* as this
   */
  template <typename T, typename R>
  static Getter wrap(R (T::*getter)()) {
    static_assert(!std::is_void_v<R>, "A getter have to return non-void");
    return
        [getter](void *p) -> Value { return (static_cast<T *>(p)->*getter)(); };
  }

  /** Wraps a member function pointer setter to accept void* as this */
  template <typename T>
  static Setter wrap(void (T::*setter)(const Value &value)) {
    return [setter](void *p, const Value &value) {
      (static_cast<T *>(p)->*setter)(value);
    };
  }
};
}  // namespace complate