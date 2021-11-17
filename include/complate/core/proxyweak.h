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

#include <memory>
#include <string>

#include "types.h"

namespace complate {

/**
 * Weak proxy for a native C++ class object. Stores a raw pointer and don't
 * manage lifetime of the object.
 *
 * This make a C++ object available inside the JavaScript Engine.
 * In order to be used, a corresponding Prototype has to be defined
 * when creating the Renderer.
 *
 * This implementation holding a raw pointer to your object,
 * you have to manage it's lifetime.
 */
class ProxyWeak {
public:
  /** Construct a ProxyWeak with given name and object pointer. */
  ProxyWeak(std::string name, void *object);

  /**
   * Construct a ProxyWeak with given name and const object pointer.
   *
   * @note ProxyWeak is not const aware, const-ness simply be casted away.
   *
   * @param name Name of the corresponding Prototype.
   * @param object Const pointer to your object.
   */
  ProxyWeak(std::string name, const void *object);

  /**
   * Construct a ProxyWeak with given name and object reference.
   *
   * @param name Name of the corresponding Prototype.
   * @param object Reference to your object. Will be stored as a Pointer.
   */
  template <typename T>
  ProxyWeak(std::string name, T &object) : ProxyWeak(move(name), &object) {}

  /**
   * Construct a ProxyWeak with the name taken from T.
   *
   * @note Because compilers don't have typeid(T).name() standardized,
   * you should not use this, when you create a shared library and
   * what to export your ProxyWeak and Prototype.
   *
   * @param object Pointer to your object.
   */
  template <typename T>
  explicit ProxyWeak(T *object) : ProxyWeak(typeid(T).name(), object) {}

  /**
   * Construct a ProxyWeak with the name taken from T.
   *
   * @note Because compilers don't have typeid(T).name() standardized,
   * you should not use this, when you create a shared library and
   * what to export your ProxyWeak and Prototype.
   *
   * @param object Reference to your object. Will be stored as a Pointer.
   */
  template <typename T>
  explicit ProxyWeak(T &object) : ProxyWeak(&object) {}

  /** Get the name of the ProxyWeak. */
  [[nodiscard]] const std::string &name() const;

  /** Get a void pointer to your instance. */
  [[nodiscard]] void *ptr() const;

  bool operator==(const ProxyWeak &other) const;

  bool operator!=(const ProxyWeak &other) const;

private:
  std::string m_name;
  void *m_object = nullptr;
};
}  // namespace complate
