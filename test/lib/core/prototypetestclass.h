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
#include <utility>

class PrototypeTestClass {
public:
  explicit PrototypeTestClass(std::string returnString)
      : m_returnString(std::move(returnString)) {}

  void setReturnString(const complate::Value &value) {
    m_returnString = value.get<std::string>();
  }

  [[nodiscard]] const complate::Array &args() const { return m_args; }

  void noReturnNoArgs() { m_cntNoReturnNoArgsCalled++; }

  void noReturnWithArgs(const complate::Array &args) {
    m_args = args;
    m_cntNoReturnWithArgsCalled++;
  }

  [[nodiscard]] std::string strReturnNoArgs() const { return m_returnString; }

  [[nodiscard]] std::string nonConstGetter() { return m_returnString; }

  std::string strReturnWithArgs(const complate::Array &args) {
    m_args = args;
    m_cntStrReturnWithArgsCalled++;
    return m_returnString;
  }

  [[nodiscard]] int cntNoReturnNoArgsCalled() const {
    return m_cntNoReturnNoArgsCalled;
  }

  [[nodiscard]] int cntNoReturnWithArgsCalled() const {
    return m_cntNoReturnWithArgsCalled;
  }

  [[nodiscard]] int cntStrReturnWithArgsCalled() const {
    return m_cntStrReturnWithArgsCalled;
  }

private:
  complate::Array m_args;
  std::string m_returnString;
  int m_cntNoReturnNoArgsCalled = 0;
  int m_cntNoReturnWithArgsCalled = 0;
  int m_cntStrReturnWithArgsCalled = 0;
};
