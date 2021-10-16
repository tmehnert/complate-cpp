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
#include <complate/core/value.h>

using namespace complate;
using namespace std;

Value::Value() = default;

Value::Value(std::nullopt_t) {}

Value::Value(Null d) : m_data(d) {}

Value::Value(Bool d) : m_data(d) {}

Value::Value(int32_t d) : m_data(forward<Number>(d)) {}

Value::Value(uint32_t d) : m_data(forward<Number>(d)) {}

Value::Value(int64_t d) : m_data(forward<Number>(d)) {}

Value::Value(double d) : m_data(forward<Number>(d)) {}

Value::Value(Number d) : m_data(d) {}

Value::Value(string d) : m_data(forward<String>(move(d))) {}

Value::Value(string_view d) : m_data(forward<String>(d)) {}

Value::Value(String text) : m_data(move(text)) {}

Value::Value(Array d) : m_data(move(d)) {}

Value::Value(const Object &d) : m_data(d) {}

Value::Value(Object &&d) noexcept : m_data(d) {}

Value::Value(Function d) : m_data(move(d)) {}

Value::Value(Proxy proxy) : m_data(move(proxy)) {}

Value::Value(ProxyWeak proxyWeak) : m_data(move(proxyWeak)) {}

Value::Value(const char *s) {
  if (s) {
    m_data = String(s);
  } else {
    m_data = nullptr;
  }
}

bool Value::operator==(const Value &other) const {
  return m_data == other.m_data;
}

bool Value::operator!=(const Value &other) const {
  return m_data != other.m_data;
}

template <>
bool Value::is<Undefined>() const {
  return holds<Undefined>();
}

template <>
bool Value::is<Null>() const {
  return holds<Null>();
}

template <>
bool Value::is<bool>() const {
  return optional<bool>().has_value();
}

template <>
bool Value::is<int32_t>() const {
  return optional<int32_t>().has_value();
}

template <>
bool Value::is<uint32_t>() const {
  return optional<uint32_t>().has_value();
}

template <>
bool Value::is<int64_t>() const {
  return optional<int64_t>().has_value();
}

template <>
bool Value::is<double>() const {
  return optional<double>().has_value();
}

template <>
bool Value::is<Number>() const {
  return optional<Number>().has_value();
}

template <>
bool Value::is<string>() const {
  return optional<String>().has_value();
}

template <>
bool Value::is<string_view>() const {
  return optional<String>().has_value();
}

template <>
bool Value::is<String>() const {
  return optional<String>().has_value();
}

template <>
bool Value::is<Array>() const {
  return optional<Array>().has_value();
}

template <>
bool Value::is<Object>() const {
  return optional<Object>().has_value();
}

template <>
bool Value::is<Function>() const {
  return optional<Function>().has_value();
}

template <>
bool Value::is<Proxy>() const {
  return optional<Proxy>().has_value();
}

template <>
bool Value::is<ProxyWeak>() const {
  return optional<ProxyWeak>().has_value();
}

template <>
bool Value::get<bool>() const {
  return optional<bool>().value();
}

template <>
int32_t Value::get<int32_t>() const {
  return optional<int32_t>().value();
}

template <>
uint32_t Value::get<uint32_t>() const {
  return optional<uint32_t>().value();
}

template <>
int64_t Value::get<int64_t>() const {
  return optional<int64_t>().value();
}

template <>
double Value::get<double>() const {
  return optional<double>().value();
}

template <>
Number Value::get<Number>() const {
  return optional<Number>().value();
}

template <>
string Value::get<string>() const {
  return optional<string>().value();
}

template <>
string_view Value::get<string_view>() const {
  return optional<string_view>().value();
}

template <>
String Value::get<String>() const {
  return optional<String>().value();
}

template <>
Array Value::get<Array>() const {
  return optional<Array>().value();
}

template <>
Object Value::get<Object>() const {
  return optional<Object>().value();
}

template <>
Function Value::get<Function>() const {
  return optional<Function>().value();
}

template <>
Proxy Value::get<Proxy>() const {
  return optional<Proxy>().value();
}

template <>
ProxyWeak Value::get<ProxyWeak>() const {
  return optional<ProxyWeak>().value();
}

template <>
optional<Bool> Value::optional() const {
  if (holds<Bool>()) {
    return std::get<Bool>(m_data);
  }

  return nullopt;
}

template <>
optional<int32_t> Value::optional() const {
  if (holds<Number>()) {
    return std::get<Number>(m_data).optional<int32_t>();
  }

  return nullopt;
}

template <>
optional<uint32_t> Value::optional() const {
  if (holds<Number>()) {
    return std::get<Number>(m_data).optional<uint32_t>();
  }

  return nullopt;
}

template <>
optional<int64_t> Value::optional() const {
  if (holds<Number>()) {
    return std::get<Number>(m_data).optional<int64_t>();
  }

  return nullopt;
}

template <>
optional<double> Value::optional() const {
  if (holds<Number>()) {
    return std::get<Number>(m_data).optional<double>();
  }

  return nullopt;
}

template <>
optional<Number> Value::optional() const {
  if (holds<Number>()) {
    return std::get<Number>(m_data);
  }

  return nullopt;
}

template <>
optional<string> Value::optional() const {
  if (holds<String>()) {
    return std::get<String>(m_data).get<string>();
  } else {
    return nullopt;
  }
}

template <>
optional<string_view> Value::optional() const {
  if (holds<String>()) {
    return std::get<String>(m_data).get<string_view>();
  } else {
    return nullopt;
  }
}

template <>
optional<String> Value::optional() const {
  if (holds<String>()) {
    return std::get<String>(m_data);
  } else {
    return nullopt;
  }
}

template <>
optional<Array> Value::optional() const {
  if (holds<Array>()) {
    return std::get<Array>(m_data);
  }

  return nullopt;
}

template <>
optional<Object> Value::optional() const {
  if (holds<Object>()) {
    return std::get<Object>(m_data);
  }

  return nullopt;
}

template <>
optional<Function> Value::optional() const {
  if (holds<Function>()) {
    return std::get<Function>(m_data);
  }

  return nullopt;
}

template <>
optional<Proxy> Value::optional() const {
  if (holds<Proxy>()) {
    return std::get<Proxy>(m_data);
  }

  return nullopt;
}

template <>
optional<ProxyWeak> Value::optional() const {
  if (holds<ProxyWeak>()) {
    return std::get<ProxyWeak>(m_data);
  }

  return nullopt;
}