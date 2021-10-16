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
#include <complate/core/proxy.h>

using namespace std;
using namespace complate;

Proxy::Proxy(string name, shared_ptr<void> object)
    : m_name(move(name)), m_object(move(object)) {}

const string &Proxy::name() const { return m_name; }

const shared_ptr<void> &Proxy::ptr() const { return m_object; }

bool Proxy::operator==(const Proxy &other) const {
  return (m_object == other.m_object) && (m_name == other.m_name);
}

bool Proxy::operator!=(const Proxy &other) const { return !operator==(other); }
