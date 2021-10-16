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
#include <vector>

#include "method.h"
#include "property.h"
#include "value.h"

namespace complate {

/**
 * Prototype of an native C++ class.
 *
 * This can be used by a Renderer to create an JavaScript prototype,
 * in order to make your class available in the Engine.
 * When you have created an registered a Prototype,
 * you are able to pass your native C++ class objects as a Proxy.
 *
 * @note Use PrototypeBuilder to build your Prototype easier.
 */
class Prototype {
public:
  /** Constructs a Prototype with given name (your class name) */
  explicit Prototype(std::string name);

  Prototype(const Prototype &other);

  ~Prototype();

  /** Add a Method */
  void addMethod(const Method &method);

  /** Add a Property */
  void addProperty(const Property &property);

  /** Get the name of the Prototype. */
  [[nodiscard]] const std::string &name() const;

  /** Get all methods */
  [[nodiscard]] const std::vector<Method> &methods() const;

  /** Get a method */
  [[nodiscard]] std::optional<Method> method(std::string_view name) const;

  /** Get all properties */
  [[nodiscard]] const std::vector<Property> &properties() const;

  /** Get a property */
  [[nodiscard]] std::optional<Property> property(std::string_view name) const;

protected:
  class Impl;

  /** Pointer to implementation */
  std::unique_ptr<Impl> m_impl;
};
}  // namespace complate