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
#include "timespandto.h"

using namespace std;

TimespanDto::TimespanDto(int amount, string unit, bool veryLate)
    : m_amount(amount), m_unit(move(unit)), m_veryLate(veryLate) {}

int TimespanDto::amount() const { return m_amount; }

const string& TimespanDto::unit() const { return m_unit; }

bool TimespanDto::veryLate() const { return m_veryLate; }
