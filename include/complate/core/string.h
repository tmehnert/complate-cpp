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

#include <string>
#include <string_view>
#include <variant>

namespace complate {

/**
 * String that can be accessed from JavaScript.
 *
 * It stores string (owning) and string_view (not owning) types.
 * It can be compared across types.
 * Regardless of the underlying type, you can get the string
 * as any string representation you want.
 */
class String {
public:
  /** construct an empty string. */
  String();

  /** Implicit constrcutred from string. */
  String(std::string s);  // NOLINT
  /** Implicit constrcutred from string_view. */
  String(std::string_view sv);  // NOLINT
  /** Implicit constrcutred. Copied. */
  String(const char *s);  // NOLINT

  /**
   * Get the string.
   *
   * @tparam T Type you want to get.
   * @return your string as type T.
   */
  template <typename T>
  T get() const = delete;

  bool operator==(const String &other) const;

  bool operator!=(const String &other) const;

  bool operator<(const String &other) const;

  /**
   * Check if the String is being stored exactly as type T.
   *
   * @attention you should normally not use this function, because
   * get() get's what you want string or string_view.
   *
   * @tparam T Type you want to check.
   * @return true, if the Value stores excaptly a type T.
   */
  template <typename T>
  [[nodiscard]] inline bool holds() const {
    return std::holds_alternative<T>(m_data);
  }

private:
  using Type = std::variant<std::string, std::string_view>;
  Type m_data;
};

template <>
[[nodiscard]] std::string String::get() const;

template <>
[[nodiscard]] std::string_view String::get() const;
}  // namespace complate
