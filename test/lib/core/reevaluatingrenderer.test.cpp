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
#include <complate/core/reevaluatingrenderer.h>

#include "catch2/catch.hpp"
#include "creatorspy.h"
#include "nooprenderer.h"
#include "stream.mock.h"

using namespace complate;
using namespace std;

TEST_CASE("ReEvaluatingRenderer", "[core]") {
  CreatorSpy creator([] { return make_unique<NoopRenderer>(); });
  ReEvaluatingRenderer renderer(creator);
  auto stream = StreamMock();

  SECTION("render map parameters") {
    const Object parameters;

    SECTION("create renderer on every call") {
      REQUIRE_FALSE(creator.wasCalled());
      renderer.render("View", parameters, stream);
      renderer.render("View", parameters, stream);
      renderer.renderToString("View", parameters);
      REQUIRE(creator.callCount() == 3);
    }
  }

  SECTION("render json parameters") {
    const string parameters;

    SECTION("create renderer on every call") {
      REQUIRE_FALSE(creator.wasCalled());
      renderer.render("View", parameters, stream);
      renderer.render("View", parameters, stream);
      renderer.renderToString("View", parameters);
      REQUIRE(creator.callCount() == 3);
    }
  }
}
