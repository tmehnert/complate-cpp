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
#include <complate/core/proxyweak.h>

using namespace std;
using namespace complate;

ProxyWeak::ProxyWeak(string name, void *object)
    : m_name(move(name)), m_object(object) {}

ProxyWeak::ProxyWeak(std::string name, const void *object)
    : ProxyWeak(move(name), (void *)object) {}

const string &ProxyWeak::name() const { return m_name; }

void *ProxyWeak::ptr() const { return m_object; }

bool ProxyWeak::operator==(const ProxyWeak &other) const {
  return (m_object == other.m_object) && (m_name == other.m_name);
}

bool ProxyWeak::operator!=(const ProxyWeak &other) const {
  return !operator==(other);
}
