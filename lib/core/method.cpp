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
#include <complate/core/method.h>

using namespace std;
using namespace complate;

class Method::Impl {
public:
  Impl(string name, Callback callback)
      : m_name(move(name)), m_callback(move(callback)) {}

  [[nodiscard]] const string &name() const { return m_name; }

  Value apply(void *object, const Array &args) {
    return (object) ? invoke(m_callback, object, args) : Value();
  }

private:
  std::string m_name;
  Callback m_callback;
};

Method::Method(string name, Callback callback)
    : m_impl(make_unique<Impl>(move(name), move(callback))) {}

Method::Method(const Method &other)
    : m_impl(make_unique<Impl>(*other.m_impl)) {}

Method::~Method() = default;

const std::string &Method::name() const { return m_impl->name(); }

Value Method::apply(void *object, const Array &args) const {
  return m_impl->apply(object, args);
}
