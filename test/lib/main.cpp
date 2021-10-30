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
#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#ifdef COMPLATE_V8_INCLUDED
#include <complate/v8/v8platform.h>
using namespace complate;
#endif

#include "catch2/catch.hpp"

int main(int argc, char* argv[]) {
#ifdef COMPLATE_V8_INCLUDED
  V8Platform platform;
  /* See v8platform.test.cpp why this has to be set */
  V8Platform::setFlags("--use-strict");
#endif

  return Catch::Session().run(argc, argv);
}