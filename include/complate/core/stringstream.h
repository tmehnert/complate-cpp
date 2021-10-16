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
#include <string>
#include <string_view>

#include "stream.h"

namespace complate {

/**
 * Stream which stores rendered output as a string.
 *
 * Use str() to get the rendered output.
 */
class StringStream : public Stream {
public:
  /**
   * Construct a StringStream.
   */
  explicit StringStream();

  ~StringStream() override;

  /**
   * Append a string to the underlying string.
   *
   * @param str String not necessarily null terminated.
   * @param len Length of the string to write.
   */
  void write(const char *str, int len) override;

  /**
   * Append a string followed by a newline to the underlying string.
   *
   * @param str String not necessarily null terminated.
   * @param len Length of the string to write.
   */
  void writeln(const char *str, int len) override;

  /** Empty implementation, does nothing. */
  void flush() override;

  /**
   * Get the rendered output.
   *
   * @return A string with the content which was written to this stream.
   */
  [[nodiscard]] const std::string &str() const;

private:
  class Impl;

  /** Pointer to implementation */
  std::unique_ptr<Impl> m_impl;
};
}  // namespace complate
