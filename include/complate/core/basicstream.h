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
#include <ostream>

#include "stream.h"

namespace complate {

/**
 * Stream which forward rendered output to a `std::basic_ostream<char>`.
 *
 * This implementation can be used with any kind of `std::basic_ostream`,
 * including but not limited to std::cerr and std::ofstream.
 * It stores the reference to the given std::basic_ostream.
 */
class BasicStream : public Stream {
public:
  /**
   * Construct a BasicStream.
   *
   * @param dest The underlying stream in which the content will be forwarded
   * (reference stored).
   */
  explicit BasicStream(std::basic_ostream<char> &dest);

  ~BasicStream() override;

  /**
   * Write a string to the underlying stream.
   *
   * @param str String not necessarily null terminated.
   * @param len Length of the string to write.
   */
  void write(const char *str, int len) override;

  /**
   * Write a string followed by a newline to the underlying stream.
   *
   * @param str String not necessarily null terminated.
   * @param len Length of the string to write.
   */
  void writeln(const char *str, int len) override;

  /** Flush the underlying stream. */
  void flush() override;

private:
  class Impl;

  /** Pointer to implementation */
  std::unique_ptr<Impl> m_impl;
};
}  // namespace complate
