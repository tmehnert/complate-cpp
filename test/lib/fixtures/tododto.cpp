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
#include "tododto.h"

using namespace std;

TodoDto::TodoDto(string what, string description, string updateLink,
                 TimespanDto timespan)
    : m_what(move(what)),
      m_description(move(description)),
      m_updateLink(move(updateLink)),
      m_timespan(move(timespan)) {}

const string &TodoDto::what() const { return m_what; }

const string &TodoDto::description() const { return m_description; }

const string &TodoDto::updateLink() const { return m_updateLink; }

const TimespanDto &TodoDto::timespan() const { return m_timespan; }
