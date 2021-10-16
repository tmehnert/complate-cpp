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
#include <complate/core/prototype.h>

#include <map>
#include <utility>

using namespace complate;
using namespace std;

class Prototype::Impl {
public:
  explicit Impl(string name) : m_name(move(name)) {}

  [[nodiscard]] const string &name() const { return m_name; }

  void addMethod(const Method &method) { m_methods.push_back(method); }

  void addProperty(const Property &property) {
    m_properties.push_back(property);
  }

  [[nodiscard]] const vector<Method> &methods() const { return m_methods; }

  [[nodiscard]] optional<Method> method(string_view name) const {
    for (const auto &m : m_methods) {
      if (m.name() == name) {
        return m;
      }
    }
    return nullopt;
  }

  [[nodiscard]] const vector<Property> &properties() const {
    return m_properties;
  }

  [[nodiscard]] optional<Property> property(string_view name) const {
    for (const auto &p : m_properties) {
      if (p.name() == name) {
        return p;
      }
    }
    return nullopt;
  }

private:
  string m_name;
  vector<Method> m_methods;
  vector<Property> m_properties;
};

Prototype::Prototype(string name) : m_impl(make_unique<Impl>(move(name))) {}

Prototype::Prototype(const Prototype &other)
    : m_impl(make_unique<Impl>(*other.m_impl)) {}

Prototype::~Prototype() = default;

void Prototype::addMethod(const Method &method) { m_impl->addMethod(method); }

void Prototype::addProperty(const Property &property) {
  m_impl->addProperty(property);
}

const string &Prototype::name() const { return m_impl->name(); }

const vector<Method> &Prototype::methods() const { return m_impl->methods(); }

optional<Method> Prototype::method(string_view name) const {
  return m_impl->method(name);
}

const vector<Property> &Prototype::properties() const {
  return m_impl->properties();
}

optional<Property> Prototype::property(string_view name) const {
  return m_impl->property(name);
}
