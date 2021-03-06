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

#include <complate/core/prototype.h>
#include <complate/core/value.h>

class Testdata {
public:
  static complate::Object bindings();
  static complate::Object forTodoList();
  static std::string forTodoListViewAsJson();

  static complate::Object forTodoListRenderBenchmark(int count);
  static complate::Object forMapperBenchmark();

  static complate::Prototype prototypeForAssets();
  static complate::Prototype prototypeForStdString();
  static complate::Prototype prototypeForTimespanDto();
  static complate::Prototype prototypeForAssigneeDto();
  static complate::Prototype prototypeForTodoDto();
  static std::vector<complate::Prototype> prototypes();
};
