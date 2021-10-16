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
#include <string_view>

namespace complate {

/**
 * Class to initialize V8 platform.
 *
 * You need to instantiate this class once in order to use V8Renderer,
 * if you don't use and initialize V8 in your application anyway.
 */
class V8Platform {
public:
  /**
   * Constructs a V8 Platform.
   *
   * Initialze the V8 Javascript Engine along with the platform.
   */
  V8Platform();

  ~V8Platform();

  /** Set flags for the V8 Javascript engine. */
  static void setFlags(std::string_view flags);

  /** Get the version of the V8 J́avascript engine. */
  static std::string_view version();

private:
  class Impl;

  /** Pointer to implementation */
  std::unique_ptr<Impl> m_impl;
};
}  // namespace complate