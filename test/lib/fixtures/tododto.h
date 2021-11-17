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

#include <string>

#include "timespandto.h"
#include "assigneedto.h"

class TodoDto {
public:
  TodoDto(std::string what, std::string description, std::string updateLink,
          TimespanDto timespan, AssigneeDto assignee);

  [[nodiscard]] const std::string &what() const;
  [[nodiscard]] const std::string &description() const;
  [[nodiscard]] const std::string &updateLink() const;
  [[nodiscard]] const TimespanDto &timespan() const;
  [[nodiscard]] const AssigneeDto &assignee() const;

private:
  std::string m_what;
  std::string m_description;
  std::string m_updateLink;
  TimespanDto m_timespan;
  AssigneeDto m_assignee;
};
