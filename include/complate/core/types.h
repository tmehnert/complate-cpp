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

#include <map>
#include <string>
#include <variant>
#include <vector>

namespace complate {
class Value;

class Function;

class Proxy;

class ProxyWeak;

class Number;

class String;

/**
 * Javascript compatible undefined value.
 *
 * @example
 * @code
 * Value v;
 */
using Undefined = std::monostate;

/**
 * Javascript compatible null value.
 *
 * @example
 * @code
 * Value v1 = Null;
 * Value v2 = std::nullptr;
 */
using Null = std::nullptr_t;

/**
 * Javascript compatible bool value.
 *
 * @example
 * @code
 * Value v = true;
 */
using Bool = bool;

/**
 * JavaScript compatible Object definition.
 *
 * @example
 * @code
 * auto car = Object{
 *   {"type", "Little car"},
 *   {"diesel", false},
 *   {"features", Array{
 *      "radio",
 *      "navigation"
 *   }}
 * };
 * car.emplace("tires", 4);
 */
using Object = std::map<std::string, Value>;

/**
 * JavaScript compatible Array definition.
 *
 * @example
 * @code
 * auto ideas = Array{
 *   42,
 *   "Code a complate adatper for C++",
 *   Object{
 *     {"what", "Write documentation"},
 *     {"why", "I hope it will be useful"}
 *   },
 *   Function{[] {
 *     return "What is a function doing in this Array?";
 *   }}
 * };
 * ideas.emplace_back(23);
 */
using Array = std::vector<Value>;

/** Variant holing all possible types of Value. */
using Type = std::variant<Undefined, Null, Bool, Number, String, Array, Object,
                          Function, Proxy, ProxyWeak>;
}  // namespace complate
