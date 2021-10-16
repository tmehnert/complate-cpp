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
#include <complate/core/property.h>

using namespace std;
using namespace complate;

class Property::Impl {
public:
  Impl(string name, Getter getter)
      : m_name(move(name)), m_getter(move(getter)) {}
  Impl(string name, Getter getter, Setter setter)
      : m_name(move(name)), m_getter(move(getter)), m_setter(move(setter)) {}

  [[nodiscard]] const string &name() const { return m_name; }

  Value get(void *object) const {
    return (object) ? invoke(m_getter, object) : Value();
  }

  void set(void *object, const Value &value) const {
    if (object && m_setter.has_value()) {
      invoke(m_setter.value(), object, value);
    }
  }

private:
  string m_name;
  Getter m_getter;
  optional<Setter> m_setter;
};

Property::Property(string name, Getter getter)
    : m_impl(make_unique<Impl>(move(name), move(getter))) {}

Property::Property(string name, Getter getter, Setter setter)
    : m_impl(make_unique<Impl>(move(name), move(getter), move(setter))) {}

Property::Property(const Property &other)
    : m_impl(make_unique<Impl>(*other.m_impl)) {}

Property::~Property() = default;

const string &complate::Property::name() const { return m_impl->name(); }

Value Property::get(void *object) const { return m_impl->get(object); }

void Property::set(void *object, const Value &value) const {
  return m_impl->set(object, value);
}
