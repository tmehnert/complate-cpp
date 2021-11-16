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

class TodoDto {
public:
  TodoDto(std::string what, std::string description, std::string needToBeDoneIn,
          bool veryLate, std::string updateLink);

  [[nodiscard]] const std::string &what() const;
  [[nodiscard]] const std::string &description() const;
  [[nodiscard]] const std::string &needToBeDoneIn() const;
  [[nodiscard]] bool veryLate() const;
  [[nodiscard]] const std::string &updateLink() const;

private:
  std::string m_what;
  std::string m_description;
  std::string m_needToBeDoneIn;
  bool m_veryLate;
  std::string m_updateLink;
};
