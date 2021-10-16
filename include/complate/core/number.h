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
#include <variant>

namespace complate {

/**
 * Number that can be accessed from JavaScript.
 *
 * It stores integer and double types.
 * It can be compared across types.
 * You can also get the Number as any type optionally,
 * then it will be checked if the stored number fits in your
 * desired return type (using get<int32_t> for example).
 *
 * @example
 * A Number created with a value of 3 and type int32_t is
 * equal a Number created with the same value, but of type
 * uint32_t or int64_t.
 */
class Number {
public:
  /** construct a zero value. */
  Number();

  /** Implicit constrcutred from int32. */
  Number(int32_t d);  // NOLINT
  /** Implicit constrcutred from uint32. */
  Number(uint32_t d);  // NOLINT
  /** Implicit constrcutred from int64_t. */
  Number(int64_t d);  // NOLINT
  /** Implicit constrcutred from double. */
  Number(double d);  // NOLINT

  /**
   * Check if this Number fits in type T.
   *
   * For example if the underlying type is int64_t
   * and the value is 3, then it fits in all data types.
   *
   * @seealso get(), optional()
   *
   * @tparam T Type you want to check.
   * @return true, if this Number fits in type T.
   */
  template <typename T>
  [[nodiscard]] bool is() const = delete;

  /**
   * Get a type T from this value or throw.
   *
   * Checks if the Number fits in type T, throw bad_optional_access otherwise.
   * Check manually via is() or use optional() to detect errors.
   *
   * @tparam T Type you want to get.
   * @return a type T of the Number, or throw if Number not fits in T.
   */
  template <typename T>
  T get() const = delete;

  /**
   * Get an optional type T from this value.
   *
   * Checks if the Number fits in type T.
   *
   * @tparam T Type you want to get.
   * @return an empty optional if Number not fit into type T.
   */
  template <typename T>
  std::optional<T> optional() const = delete;

  bool operator==(const Number &other) const;

  bool operator!=(const Number &other) const;

  /**
   * Check if the Number is being stored exactly as type T.
   *
   * Checks the underlying storage type for this Number.
   * For example holds<int32_t>() for an underlying Type int64_t
   * is false, no matter if the value fits in int32_t or not.
   *
   * @attention For most cases Use Number::is instead, as the result
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
   * Checks the underlying storage type for this Number.
   * For example exactly<int32_t>() for an underlying Type int64_t
   * will throw, no matter if the value fits in int32_t or not.
   *
   * @attention For most cases Use Number::get instead.
   *
   * @tparam T Type you want to get.
   * @return Reference to the underlying type.
   */
  template <typename T>
  inline const T &exactly() const {
    return std::get<T>(m_data);
  }

private:
  using Type = std::variant<int32_t, uint32_t, int64_t, double>;
  Type m_data;
};

template <>
bool Number::is<int32_t>() const;

template <>
bool Number::is<uint32_t>() const;

template <>
bool Number::is<int64_t>() const;

template <>
bool Number::is<double>() const;

template <>
[[nodiscard]] int32_t Number::get<int32_t>() const;

template <>
[[nodiscard]] uint32_t Number::get<uint32_t>() const;

template <>
[[nodiscard]] int64_t Number::get<int64_t>() const;

template <>
[[nodiscard]] double Number::get<double>() const;

template <>
[[nodiscard]] std::optional<int32_t> Number::optional() const;

template <>
[[nodiscard]] std::optional<uint32_t> Number::optional() const;

template <>
[[nodiscard]] std::optional<int64_t> Number::optional() const;

template <>
[[nodiscard]] std::optional<double> Number::optional() const;
}  // namespace complate
