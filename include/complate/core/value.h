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

#include <cstdint>
#include <optional>

#include "exception.h"
#include "function.h"
#include "number.h"
#include "proxy.h"
#include "proxyweak.h"
#include "string.h"  // NOLINT
#include "types.h"

namespace complate {

/**
 * Javascript compatible Value.
 *
 * The variant of all types which can be made available to the JavaScript
 * Engine. This class has many constructors that are implicit,
 * just assign your type. It's also compatiable with std::optional<T>,
 * turning the Value into Undefined, when the std::optional is empty.
 *
 * @example
 * @code
 * Value v1 = std::string_view("foo");
 * Value v2 = std::optional<std::string>("foo");
 * v1 == v2;
 * Value obj = Object{
 *   { "foo", "bar" },
 *   { "somearray", Array{1, true, 2.3} }
 * };
 */
class Value {
public:
  /** Default constructs an undefined Value */
  Value();

  /** Implicit constructed from nullopt. Represent a undefined value. */
  Value(std::nullopt_t);  // NOLINT
  /** Implicit constructed from nullptr. Represent a null value. */
  Value(Null d);  // NOLINT
  /** Implicit constructed from bool. */
  Value(Bool d);  // NOLINT
  /** Implicit constructed from int32. */
  Value(int32_t d);  // NOLINT
  /** Implicit constructed from uint32. */
  Value(uint32_t d);  // NOLINT
  /** Implicit constructed from int64_t. */
  Value(int64_t d);  // NOLINT
  /** Implicit constructed from double. */
  Value(double d);  // NOLINT
  /** Implicit constructed from Number. */
  Value(Number d);  // NOLINT
  /** Implicit constructed from string. */
  Value(std::string d);  // NOLINT
  /** Implicit constructed from string_view, not owning the string */
  Value(std::string_view d);  // NOLINT
  /** Implicit constructed from Text. */
  Value(String text);  // NOLINT
  /** Implicit constructed from Array. */
  Value(Array d);  // NOLINT
  /** Implicit constructed by copying an Object. */
  Value(const Object &d);  // NOLINT
  /** Implicit constructed by moving an Object. */
  Value(Object &&d) noexcept;  // NOLINT
  /** Implicit constructed from Function */
  Value(Function d);  // NOLINT
  /** Implicit constructed from Proxy */
  Value(Proxy proxy);  // NOLINT
  /** Implicit constructed from Proxy */
  Value(ProxyWeak proxyWeak);  // NOLINT

  /** Implicit constructed from const char *. Copied and stored as a string */
  Value(const char *s);  // NOLINT

  /** Implicit constructed from optional<T>. Empty optional have value Undefined
   */
  template <typename T,
            typename = std::enable_if_t<std::is_constructible_v<Value, T>>>
  Value(std::optional<T> v) {  // NOLINT
    if (v.has_value()) {
      *this = Value(v.value());
    }
  }

  bool operator==(const Value &other) const;

  bool operator!=(const Value &other) const;

  /**
   * Check if this Value can be used as type T.
   *
   * For example for numbers it checks if the value
   * can be converted into type T.
   *
   * @seealso get(), optional()
   *
   * @tparam T Type you want to check.
   * @return true, if this Value can be converted to type T.
   */
  template <typename T>
  [[nodiscard]] bool is() const = delete;

  /**
   * Get a type T from this value or throw.
   *
   * Checks if the Value fits in type T, throw bad_optional_access otherwise.
   * Check manually via is() or use optional() to detect errors.
   *
   * @tparam T Type you want to get.
   * @return a type T, or throw if Value not fits in T, is Null or is Undefined.
   */
  template <typename T>
  [[nodiscard]] T get() const = delete;

  /**
   * Get an optional type T from this value.
   *
   * Checks if the Value can be converted to type T
   * and the Value is not undefined or null.
   *
   * @tparam T Type you want to get.
   * @return an empty optional if Value is undefined, null or not convertable.
   */
  template <typename T>
  [[nodiscard]] std::optional<T> optional() const = delete;

  /**
   * Check if the Value is being stored exactly as type T.
   *
   * Checks the underlying storage type for this Value.
   * For example holds<int32_t>() for an underlying Type int64_t
   * is false, no matter if the value fits in int32_t or not.
   *
   * @attention For most cases Use Value::is instead, as the result
   * can differ for various Renderer implementations.
   *
   * @tparam T Type you want to check.
   * @return true, if the Value stores excaptly a type T.
   */
  template <typename T>
  [[nodiscard]] inline bool holds() const {
    return std::holds_alternative<T>(m_data);
  }

  /**
   * Get the underlying Type if it's an type T, throw otherwise.
   *
   * Checks the underlying storage type for this Value.
   * For example holds<int32_t>() for an underlying Type int64_t
   * will throw, no matter if the value fits in int32_t or not.
   *
   * @attention For most cases Use Value::get instead.
   *
   * @tparam T Type you want to get.
   * @return Reference to the underlying type.
   */
  template <typename T>
  [[nodiscard]] inline const T &exactly() const {
    return std::get<T>(m_data);
  }

private:
  Type m_data;
};

template <>
bool Value::is<Undefined>() const;

template <>
bool Value::is<Null>() const;

template <>
bool Value::is<Bool>() const;

template <>
bool Value::is<int32_t>() const;

template <>
bool Value::is<uint32_t>() const;

template <>
bool Value::is<int64_t>() const;

template <>
bool Value::is<double>() const;

template <>
bool Value::is<Number>() const;

template <>
bool Value::is<std::string>() const;

template <>
bool Value::is<std::string_view>() const;

template <>
bool Value::is<String>() const;

template <>
bool Value::is<Array>() const;

template <>
bool Value::is<Object>() const;

template <>
bool Value::is<Function>() const;

template <>
bool Value::is<Proxy>() const;

template <>
bool Value::is<ProxyWeak>() const;

template <>
[[nodiscard]] Bool Value::get<Bool>() const;

template <>
[[nodiscard]] int32_t Value::get<int32_t>() const;

template <>
[[nodiscard]] uint32_t Value::get<uint32_t>() const;

template <>
[[nodiscard]] int64_t Value::get<int64_t>() const;

template <>
[[nodiscard]] double Value::get<double>() const;

template <>
[[nodiscard]] Number Value::get<Number>() const;

template <>
[[nodiscard]] std::string Value::get<std::string>() const;

template <>
[[nodiscard]] std::string_view Value::get<std::string_view>() const;

template <>
[[nodiscard]] String Value::get<String>() const;

template <>
[[nodiscard]] Array Value::get<Array>() const;

template <>
[[nodiscard]] Object Value::get<Object>() const;

template <>
[[nodiscard]] Function Value::get<Function>() const;

template <>
[[nodiscard]] Proxy Value::get<Proxy>() const;

template <>
[[nodiscard]] ProxyWeak Value::get<ProxyWeak>() const;

template <>
[[nodiscard]] std::optional<Bool> Value::optional() const;

template <>
[[nodiscard]] std::optional<int32_t> Value::optional() const;

template <>
[[nodiscard]] std::optional<uint32_t> Value::optional() const;

template <>
[[nodiscard]] std::optional<int64_t> Value::optional() const;

template <>
[[nodiscard]] std::optional<double> Value::optional() const;

template <>
[[nodiscard]] std::optional<Number> Value::optional() const;

template <>
[[nodiscard]] std::optional<std::string> Value::optional() const;

template <>
[[nodiscard]] std::optional<std::string_view> Value::optional() const;

template <>
[[nodiscard]] std::optional<String> Value::optional() const;

template <>
[[nodiscard]] std::optional<Array> Value::optional() const;

template <>
[[nodiscard]] std::optional<Object> Value::optional() const;

template <>
[[nodiscard]] std::optional<Function> Value::optional() const;

template <>
[[nodiscard]] std::optional<Proxy> Value::optional() const;

template <>
[[nodiscard]] std::optional<ProxyWeak> Value::optional() const;
}  // namespace complate
