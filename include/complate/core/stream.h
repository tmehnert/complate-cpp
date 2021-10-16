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

namespace complate {

/**
 * Stream interface used to render output.
 *
 * This interface used by `complate::Renderer` to write generated HTML.
 * Several implementations included, but when you need your own
 * Stream implementation, then subclass this interface.
 */
class Stream {
public:
  virtual ~Stream() = default;

  /**
   * Write a string to the stream.
   *
   * @param str String not necessarily null termined.
   * @param len Length of the string to write.
   */
  virtual void write(const char *str, int len) = 0;

  /**
   * Write a string to the stream followed by a newline.
   *
   * @param str String not necessarily null termined.
   * @param len Length of the string to write.
   */
  virtual void writeln(const char *str, int len) = 0;

  /**
   * Flush the stream.
   *
   * When your `Stream` implementation doesn't suuport
   * or require to `flush()`, then just leave implementation
   * empty.
   */
  virtual void flush() = 0;
};
}  // namespace complate
