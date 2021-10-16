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
#include <complate/core/number.h>

#include "catch2/catch.hpp"

using namespace complate;
using namespace std;

TEST_CASE("Number", "[core]") {
  SECTION("traits") {
    REQUIRE(is_copy_constructible_v<Number>);
    REQUIRE(is_copy_assignable_v<Number>);
    REQUIRE(is_move_constructible_v<Number>);
    REQUIRE(is_move_assignable_v<Number>);
  }

  SECTION("construct default") {
    const Number number;
    SECTION("holds int32_t") { REQUIRE(number.holds<int32_t>()); }
    SECTION("has value zero") { REQUIRE(number == 0); }
  }

  SECTION("construct with int32_t") {
    const Number number = (int32_t)3;
    SECTION("holds int32_t") { REQUIRE(number.holds<int32_t>()); }
    SECTION("can compared with") {
      SECTION("Number") {
        REQUIRE(number == Number(3));
        REQUIRE(number != Number(4));
      }
      SECTION("int32_t") {
        REQUIRE(number == (int32_t)3);
        REQUIRE(number != (int32_t)4);
      }
      SECTION("uint32_t") {
        REQUIRE(number == (uint32_t)3);
        REQUIRE(number != (uint32_t)4);
      }
      SECTION("int64_t") {
        REQUIRE(number == (int64_t)3);
        REQUIRE(number != (int64_t)4);
      }
    }
    SECTION("can be get as") {
      SECTION("int32_t") {
        REQUIRE(number.is<int32_t>());
        REQUIRE(number.get<int32_t>() == (int32_t)3);
        REQUIRE(number.optional<int32_t>() == (int32_t)3);
      }
      SECTION("uint32_t") {
        REQUIRE(number.is<uint32_t>());
        REQUIRE(number.get<uint32_t>() == (uint32_t)3);
        REQUIRE(number.optional<uint32_t>() == (uint32_t)3);
      }
      SECTION("int64_t") {
        REQUIRE(number.is<int64_t>());
        REQUIRE(number.get<int64_t>() == (int64_t)3);
        REQUIRE(number.optional<int64_t>() == (int64_t)3);
      }
      SECTION("double") {
        REQUIRE(number.is<double>());
        REQUIRE(number.get<double>() == 3.0);
        REQUIRE(number.optional<double>() == 3.0);
      }
    }
    SECTION("cannot be get as") {
      SECTION("uint32_t, when value is negative") {
        const Number cannot = (int32_t)-5;
        REQUIRE_FALSE(cannot.is<uint32_t>());
        REQUIRE_THROWS_AS(cannot.get<uint32_t>(), bad_optional_access);
        REQUIRE_FALSE(cannot.optional<uint32_t>().has_value());
      }
    }
  }

  SECTION("construct with uint32_t") {
    const Number number = (uint32_t)3;
    SECTION("holds uint32_t") { REQUIRE(number.holds<uint32_t>()); }
    SECTION("can compared with") {
      SECTION("Number") {
        REQUIRE(number == Number(3));
        REQUIRE(number != Number(4));
      }
      SECTION("int32_t") {
        REQUIRE(number == (int32_t)3);
        REQUIRE(number != (int32_t)4);
      }
      SECTION("uint32_t") {
        REQUIRE(number == (uint32_t)3);
        REQUIRE(number != (uint32_t)4);
      }
      SECTION("int64_t") {
        REQUIRE(number == (int64_t)3);
        REQUIRE(number != (int64_t)4);
      }
    }
    SECTION("can be get as") {
      SECTION("int32_t") {
        REQUIRE(number.is<int32_t>());
        REQUIRE(number.get<int32_t>() == (int32_t)3);
        REQUIRE(number.optional<int32_t>() == (int32_t)3);
      }
      SECTION("uint32_t") {
        REQUIRE(number.is<uint32_t>());
        REQUIRE(number.get<uint32_t>() == (uint32_t)3);
        REQUIRE(number.optional<uint32_t>() == (uint32_t)3);
      }
      SECTION("int64_t") {
        REQUIRE(number.is<int64_t>());
        REQUIRE(number.get<int64_t>() == (int64_t)3);
        REQUIRE(number.optional<int64_t>() == (int64_t)3);
      }
      SECTION("double") {
        REQUIRE(number.is<double>());
        REQUIRE(number.get<double>() == 3.0);
        REQUIRE(number.optional<double>() == 3.0);
      }
    }
    SECTION("cannot be get as") {
      SECTION("int32_t, when value exceed positive range") {
        const Number cannot = (uint32_t)numeric_limits<uint32_t>::max();
        REQUIRE_FALSE(cannot.is<int32_t>());
        REQUIRE_THROWS_AS(cannot.get<int32_t>(), bad_optional_access);
        REQUIRE_FALSE(cannot.optional<int32_t>().has_value());
      }
    }
  }

  SECTION("construct with int64_t") {
    const Number number = (int64_t)3;
    SECTION("holds int64_t") { REQUIRE(number.holds<int64_t>()); }
    SECTION("can compared with") {
      SECTION("Number") {
        REQUIRE(number == Number(3));
        REQUIRE(number != Number(4));
      }
      SECTION("int32_t") {
        REQUIRE(number == (int32_t)3);
        REQUIRE(number != (int32_t)4);
      }
      SECTION("uint32_t") {
        REQUIRE(number == (uint32_t)3);
        REQUIRE(number != (uint32_t)4);
      }
      SECTION("int64_t") {
        REQUIRE(number == (int64_t)3);
        REQUIRE(number != (int64_t)4);
      }
    }
    SECTION("can be get as") {
      SECTION("int32_t") {
        REQUIRE(number.is<int32_t>());
        REQUIRE(number.get<int32_t>() == (int32_t)3);
        REQUIRE(number.optional<int32_t>() == (int32_t)3);
      }
      SECTION("uint32_t") {
        REQUIRE(number.is<uint32_t>());
        REQUIRE(number.get<uint32_t>() == (uint32_t)3);
        REQUIRE(number.optional<uint32_t>() == (uint32_t)3);
      }
      SECTION("int64_t") {
        REQUIRE(number.is<int64_t>());
        REQUIRE(number.get<int64_t>() == (int64_t)3);
        REQUIRE(number.optional<int64_t>() == (int64_t)3);
      }
      SECTION("double") {
        REQUIRE(number.is<double>());
        REQUIRE(number.get<double>() == 3.0);
        REQUIRE(number.optional<double>() == 3.0);
      }
    }
    SECTION("cannot be get as") {
      SECTION("int32_t, when value exceed positive range") {
        const Number cannot = (int64_t)numeric_limits<int64_t>::max();
        REQUIRE_FALSE(cannot.is<int32_t>());
        REQUIRE_THROWS_AS(cannot.get<int32_t>(), bad_optional_access);
        REQUIRE_FALSE(cannot.optional<int32_t>().has_value());
      }
      SECTION("int32_t, when value exceed negative range") {
        const Number cannot = (int64_t)numeric_limits<int64_t>::min();
        REQUIRE_FALSE(cannot.is<int32_t>());
        REQUIRE_THROWS_AS(cannot.get<int32_t>(), bad_optional_access);
        REQUIRE_FALSE(cannot.optional<int32_t>().has_value());
      }
      SECTION("uint32_t, when value is negative") {
        const Number cannot = (int64_t)-1;
        REQUIRE_FALSE(cannot.is<uint32_t>());
        REQUIRE_THROWS_AS(cannot.get<uint32_t>(), bad_optional_access);
        REQUIRE_FALSE(cannot.optional<uint32_t>().has_value());
      }
    }
  }

  SECTION("construct with double") {
    const Number number = (double)3.1415;
    SECTION("holds double") { REQUIRE(number.holds<double>()); }
    SECTION("can compared with") {
      SECTION("Number") {
        REQUIRE(number == Number(3.1415));
        REQUIRE(number != Number(4));
      }
      SECTION("int32_t") {
        REQUIRE(number != (int32_t)3);
        REQUIRE(number != (int32_t)4);
      }
      SECTION("uint32_t") {
        REQUIRE(number != (uint32_t)3);
        REQUIRE(number != (uint32_t)4);
      }
      SECTION("int64_t") {
        REQUIRE(number != (int64_t)3);
        REQUIRE(number != (int64_t)4);
      }
      SECTION("double") {
        REQUIRE(number == 3.1415);
        REQUIRE(number != 4.1415);
      }
    }
    SECTION("can be get as") {
      SECTION("int32_t") {
        REQUIRE(number.is<int32_t>());
        REQUIRE(number.get<int32_t>() == (int32_t)3);
        REQUIRE(number.optional<int32_t>() == (int32_t)3);
      }
      SECTION("uint32_t") {
        REQUIRE(number.is<uint32_t>());
        REQUIRE(number.get<uint32_t>() == (uint32_t)3);
        REQUIRE(number.optional<uint32_t>() == (uint32_t)3);
      }
      SECTION("int64_t") {
        REQUIRE(number.is<int64_t>());
        REQUIRE(number.get<int64_t>() == (int64_t)3);
        REQUIRE(number.optional<int64_t>() == (int64_t)3);
      }
      SECTION("double") {
        REQUIRE(number.is<double>());
        REQUIRE(number.get<double>() == 3.1415);
        REQUIRE(number.optional<double>() == 3.1415);
      }
    }
    SECTION("cannot be get as") {
      SECTION("int32_t when value exceed negative range") {
        const Number cannot = -numeric_limits<double>::max();
        REQUIRE_FALSE(cannot.is<int32_t>());
        REQUIRE_THROWS_AS(cannot.get<int32_t>(), bad_optional_access);
        REQUIRE_FALSE(cannot.optional<int32_t>().has_value());
      }
      SECTION("int32_t when value exceed positive range") {
        const Number cannot = numeric_limits<double>::max();
        REQUIRE_FALSE(cannot.is<int32_t>());
        REQUIRE_THROWS_AS(cannot.get<int32_t>(), bad_optional_access);
        REQUIRE_FALSE(cannot.optional<int32_t>().has_value());
      }
      SECTION("uint32_t when value is negative") {
        const Number cannot = (double)-1.0;
        REQUIRE_FALSE(cannot.is<uint32_t>());
        REQUIRE_THROWS_AS(cannot.get<uint32_t>(), bad_optional_access);
        REQUIRE_FALSE(cannot.optional<uint32_t>().has_value());
      }
      SECTION("int32_t when value exceed positive range") {
        const Number cannot = numeric_limits<double>::max();
        REQUIRE_FALSE(cannot.is<uint32_t>());
        REQUIRE_THROWS_AS(cannot.get<uint32_t>(), bad_optional_access);
        REQUIRE_FALSE(cannot.optional<uint32_t>().has_value());
      }
      SECTION("int64_t when value exceed negative range") {
        const Number cannot = -numeric_limits<double>::max();
        REQUIRE_FALSE(cannot.is<int64_t>());
        REQUIRE_THROWS_AS(cannot.get<int64_t>(), bad_optional_access);
        REQUIRE_FALSE(cannot.optional<int64_t>().has_value());
      }
      SECTION("int64_t when value exceed positive range") {
        const Number cannot = numeric_limits<double>::max();
        REQUIRE_FALSE(cannot.is<int64_t>());
        REQUIRE_THROWS_AS(cannot.get<int64_t>(), bad_optional_access);
        REQUIRE_FALSE(cannot.optional<int64_t>().has_value());
      }
    }
  }
}