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

class AssigneeDto {
public:
  AssigneeDto(std::string forename, std::string lastname);

  [[nodiscard]] const std::string& forename() const;
  [[nodiscard]] const std::string& lastname() const;

private:
  std::string m_forename;
  std::string m_lastname;
};

