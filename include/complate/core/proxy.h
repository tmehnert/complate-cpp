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
 * Proxy for a native C++ class object. Manages lifetime of the object.
 *
 * This make a C++ object available inside the JavaScript Engine.
 * In order to be used, a corresponding Prototype has to be defined
 * when creating the Renderer.
 *
 * This implementation holding a shared_ptr to your object and manages
 * it's lifetime.
 */
class Proxy {
public:
  /** Construct a Proxy with given name and object */
  Proxy(std::string name, std::shared_ptr<void> object);

  /**
   * Construct a Proxy with the name taken from T.
   *
   * @note Because compilers don't have typeid(T).name() standardized,
   * you should not use this, when you create a shared library and
   * what to export your Proxy and Prototype.
   *
   * @param object Shared pointer of your instance.
   */
  template <typename T>
  explicit Proxy(std::shared_ptr<T> object) : Proxy(typeid(T).name(), object) {}

  /** Get the name of the Proxy. */
  [[nodiscard]] const std::string &name() const;

  /** Get a void pointer to your instance. */
  [[nodiscard]] const std::shared_ptr<void> &ptr() const;

  bool operator==(const Proxy &other) const;

  bool operator!=(const Proxy &other) const;

private:
  std::string m_name;
  std::shared_ptr<void> m_object;
};
}  // namespace complate
