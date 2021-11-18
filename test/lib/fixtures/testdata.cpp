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
#include "testdata.h"

#include <complate/core/prototypebuilder.h>

#include <algorithm>

#include "assets.h"
#include "assigneedto.h"
#include "timespandto.h"
#include "tododto.h"

using namespace complate;
using namespace std;

Object Testdata::bindings() {
  return {{"renderedBy", "complate-cpp"},
          {"getRendererLink",
           Function{[] { return "https://github.com/tmehnert/complate-cpp"; }}},
          {"assets",
           Proxy("Assets",
                 make_shared<Assets>(
                     "https://cdn.jsdelivr.net/npm/bootstrap@5.1.0/dist"))}};
}

Object Testdata::forTodoList() {
  return {
      {"todos",
       Array{
           Object{
               {"what", "Change the tires of your car"},
               {"description", "You stored the tires at your mom's house."},
               {"updateLink", "https://example.org/todos/4/update"},
               {"timespan",
                Object{{"amount", 9}, {"unit", "days"}, {"veryLate", true}}},
               {"assignee", Object{{"forename", "John"}, {"lastname", "Doe"}}}},
           Proxy{"TodoDto",
                 make_shared<TodoDto>("Book a hotel for next summer",
                                      "Hopefully our situation is then better.",
                                      "https://example.org/todos/5/update",
                                      TimespanDto(11, "months", false),
                                      AssigneeDto("Jane", "Doe"))}}}};
}

string Testdata::forTodoListViewAsJson() {
  return R"json(
    {
      "todos": [
        {
          "what": "Change the tires of your car",
          "description": "You stored the tires at your mom's house.",
          "updateLink": "https://example.org/todos/4/update",
          "timespan": {
            "amount": 9,
            "unit": "days",
            "veryLate": true
          },
          "assignee": {
            "forename": "John",
            "lastname": "Doe"
          }
        },
        {
          "what": "Book a hotel for next summer",
          "description": "Hopefully our situation is then better.",
          "updateLink": "https://example.org/todos/5/update",
          "timespan": {
            "amount": 11,
            "unit": "months",
            "veryLate": false
          },
          "assignee": {
            "forename": "Jane",
            "lastname": "Doe"
          }
        }
      ]
    }
  )json";
}

Object Testdata::forTodoListRenderBenchmark(int count) {
  Value todo =
      Object{{"what", "Change the tires of your car"},
             {"description", "You stored the tires at your mom's house."},
             {"updateLink", "https://exmaple.org/todos/4/update"},
             {"timespan",
              Object{{"amount", 9}, {"unit", "days"}, {"veryLate", true}}},
             {"assignee", Object{{"forename", "John"}, {"lastname", "Doe"}}}};

  return Object{{"todos", Array(count, todo)}};
}

Object Testdata::forMapperBenchmark() {
  return {
      {"selfLink", "https://example.org/people"},
      {"people",
       Array{Object{{"forename", string("John")},
                    {"lastname", string_view("Doo")},
                    {"age", 23},
                    {"address", Object{{"street", "Down Street"}, {"no", 123}}},
                    {"hobbies", Array{"Coding", "Driving"}}},
             Object{{"forename", string("Jane")},
                    {"lastname", string_view("Doo")},
                    {"age", 21},
                    {"address", Object{{"street", "Down Street"}, {"no", 123}}},
                    {"hobbies", Array{"Coding", "Shopping", "Dancing"}}},
             Object{{"forename", string("Little")},
                    {"lastname", string_view("Doo")},
                    {"age", 4},
                    {"address", Object{{"street", "Down Street"}, {"no", 123}}},
                    {"hobbies", Array{"Eating", "Sleeping"}}}}}};
}

Prototype Testdata::prototypeForAssets() {
  return PrototypeBuilder<Assets>("Assets")
      .method<string>(
          "link",
          [](const Assets &assets, const Array &args) {
            return assets.link(args.at(0).optional<string>().value());
          })
      .build();
}

Prototype Testdata::prototypeForStdString() {
  return PrototypeBuilder<string>("std::string")
      .property("text", &string::c_str,
                [](string &str, const Value &value) {
                  str.assign(value.get<string>());
                })
      .property<int>("length", &string::length)
      .method("clear", &string::clear)
      .method("empty", &string::empty)
      .build();
}

Prototype Testdata::prototypeForTimespanDto() {
  return PrototypeBuilder<TimespanDto>("TimespanDto")
      .property("amount", &TimespanDto::amount)
      .property("unit", &TimespanDto::unit)
      .property("veryLate", &TimespanDto::veryLate)
      .build();
}

Prototype Testdata::prototypeForAssigneeDto() {
  return PrototypeBuilder<AssigneeDto>("AssigneeDto")
      .property("forename", &AssigneeDto::forename)
      .property("lastname", &AssigneeDto::lastname)
      .build();
}

Prototype Testdata::prototypeForTodoDto() {
  return PrototypeBuilder<TodoDto>("TodoDto")
      .property("what", &TodoDto::what)
      .property("description", &TodoDto::description)
      .property("updateLink", &TodoDto::updateLink)
      .property<ProxyWeak>("timespan",
                           [](const TodoDto &todo) {
                             return ProxyWeak("TimespanDto", todo.timespan());
                           })
      .property<Proxy>("assignee",
                       [](const TodoDto &todo) {
                         return Proxy("AssigneeDto", make_shared<AssigneeDto>(
                                                         todo.assignee()));
                       })
      .build();
}

vector<Prototype> Testdata::prototypes() {
  return {prototypeForAssets(), prototypeForStdString(),
          prototypeForTimespanDto(), prototypeForAssigneeDto(),
          prototypeForTodoDto()};
}
