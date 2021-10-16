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
#include <complate/core/stringstream.h>

using namespace complate;
using namespace std;

class StringStream::Impl {
public:
  explicit Impl() = default;

  inline void write(const char *str, int len) { m_dest.append(str, len); }

  inline void writeln(const char *str, int len) {
    m_dest.append(str, len);
    m_dest.append("\n", 1);
  }

  [[nodiscard]] const std::string &str() const { return m_dest; }

private:
  string m_dest;
};

StringStream::StringStream() : m_impl(make_unique<Impl>()) {}
StringStream::~StringStream() = default;

void StringStream::write(const char *str, int len) { m_impl->write(str, len); }

void StringStream::writeln(const char *str, int len) {
  m_impl->writeln(str, len);
}

void StringStream::flush() {}

const std::string &StringStream::str() const { return m_impl->str(); }
