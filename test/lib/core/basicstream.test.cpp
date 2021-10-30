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
#include <complate/core/basicstream.h>

#include <algorithm>
#include <fstream>

#include "catch2/catch.hpp"

using namespace Catch::Matchers;
using namespace complate;
using namespace std;

TEST_CASE("BasicStream", "[core]") {
  std::string filename = tmpnam(nullptr);
  filename.erase(remove(filename.begin(), filename.end(), '\\'),
                 filename.end());
  ofstream ofs = ofstream(filename);
  auto stream = make_shared<BasicStream>(ofs);

  auto readFile = [&filename] {
    ifstream ifs = ifstream(filename);
    return string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
  };

  SECTION("write") {
    SECTION("appending to stream") {
      stream->write("7 chars", 7);
      stream->flush();
      REQUIRE_THAT(readFile(), Equals("7 chars"));
      stream->write(" + 11 chars", 11);
      stream->flush();
      REQUIRE_THAT(readFile(), Equals("7 chars + 11 chars"));
    }

    SECTION("not flush the stream") {
      stream->write("7 chars", 7);
      REQUIRE_THAT(readFile(), Equals(""));
    }
  }

  SECTION("writeln") {
    SECTION("appending to stream with newline") {
      stream->writeln("7 chars", 7);
      stream->flush();
      REQUIRE_THAT(readFile(), Equals("7 chars\n"));
      stream->writeln(" + 11 chars", 11);
      stream->flush();
      REQUIRE_THAT(readFile(), Equals("7 chars\n + 11 chars\n"));
    }

    SECTION("not flush the stream") {
      stream->writeln("7 chars", 7);
      REQUIRE_THAT(readFile(), Equals(""));
    }
  }

  SECTION("flush") {
    SECTION("flush the stream") {
      stream->write("7 chars", 7);
      REQUIRE_THAT(readFile(), Equals(""));
      stream->flush();
      REQUIRE_THAT(readFile(), Equals("7 chars"));
    }
  }

  remove(filename.c_str());
}