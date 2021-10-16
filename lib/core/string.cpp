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
#include <complate/core/string.h>  // NOLINT

using namespace complate;
using namespace std;

String::String() : m_data(string_view()) {}

String::String(string s) : m_data(move(s)) {}

String::String(string_view sv) : m_data(sv) {}

String::String(const char *s) : m_data(string(s)) {}

template <>
string String::get() const {
  if (holds<string_view>()) {
    auto sv = std::get<string_view>(m_data);
    return {sv.data(), sv.length()};
  } else {
    return std::get<string>(m_data);
  }
}

template <>
string_view String::get() const {
  if (holds<string_view>()) {
    return std::get<string_view>(m_data);
  } else {
    return std::get<string>(m_data);
  }
}

bool String::operator==(const String &other) const {
  return get<string_view>() == other.get<string_view>();
}

bool String::operator!=(const String &other) const {
  return get<string_view>() != other.get<string_view>();
}

bool String::operator<(const String &other) const {
  return get<string_view>() < other.get<string_view>();
}
