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
#include "creatorspy.h"

using namespace complate;
using namespace std;

CreatorSpy::CreatorSpy(Renderer::Creator creator)
    : m_creator(move(creator)), m_callCount(0) {}

int CreatorSpy::callCount() { return m_callCount; }

bool CreatorSpy::wasCalled() { return m_callCount > 0; }

Renderer::Creator CreatorSpy::creator() {
  return [&] {
    ++m_callCount;
    return m_creator();
  };
}

CreatorSpy::operator Renderer::Creator() { return creator(); }
