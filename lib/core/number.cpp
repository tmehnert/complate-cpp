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
#include <complate/core/number.h>

#include <cmath>

using namespace complate;
using namespace std;

Number::Number() : m_data(0) {}

Number::Number(int32_t d) : m_data(d) {}

Number::Number(uint32_t d) : m_data(d) {}

Number::Number(int64_t d) : m_data(d) {}

Number::Number(double d) : m_data(d) {}

template <>
bool Number::is<int32_t>() const {
  return optional<int32_t>().has_value();
}

template <>
bool Number::is<uint32_t>() const {
  return optional<uint32_t>().has_value();
}

template <>
bool Number::is<int64_t>() const {
  return optional<int64_t>().has_value();
}

template <>
bool Number::is<double>() const {
  return optional<double>().has_value();
}

template <>
int32_t Number::get<int32_t>() const {
  return optional<int32_t>().value();
}

template <>
uint32_t Number::get<uint32_t>() const {
  return optional<uint32_t>().value();
}

template <>
int64_t Number::get<int64_t>() const {
  return optional<int64_t>().value();
}

template <>
double Number::get<double>() const {
  return optional<double>().value();
}

template <>
optional<int32_t> Number::optional() const {
  if (holds<int32_t>()) {
    return std::get<int32_t>(m_data);
  }

  if (holds<uint32_t>()) {
    uint32_t val = std::get<uint32_t>(m_data);
    if (val <= (uint32_t)numeric_limits<int32_t>::max()) {
      return (int32_t)val;
    }
  }

  if (holds<int64_t>()) {
    int64_t val = std::get<int64_t>(m_data);
    if (val >= numeric_limits<int32_t>::min() &&
        val <= numeric_limits<int32_t>::max()) {
      return (int32_t)val;
    }
  }

  if (holds<double>()) {
    double val = std::get<double>(m_data);
    if (val >= numeric_limits<int32_t>::min() &&
        val <= numeric_limits<int32_t>::max()) {
      return (int32_t)val;
    }
  }

  return nullopt;
}

template <>
optional<uint32_t> Number::optional() const {
  if (holds<uint32_t>()) {
    return std::get<uint32_t>(m_data);
  }

  if (holds<int32_t>()) {
    int32_t val = std::get<int32_t>(m_data);
    if (val >= (int32_t)numeric_limits<uint32_t>::min()) {
      return (uint32_t)val;
    }
  }

  if (holds<int64_t>()) {
    int64_t val = std::get<int64_t>(m_data);
    if (val >= numeric_limits<uint32_t>::min() &&
        val <= numeric_limits<uint32_t>::max()) {
      return (uint32_t)val;
    }
  }

  if (holds<double>()) {
    double val = std::get<double>(m_data);
    if (val >= numeric_limits<uint32_t>::min() &&
        val <= numeric_limits<uint32_t>::max()) {
      return (uint32_t)val;
    }
  }

  return nullopt;
}

template <>
optional<int64_t> Number::optional() const {
  if (holds<int64_t>()) {
    return std::get<int64_t>(m_data);
  }

  if (holds<int32_t>()) {
    int32_t val = std::get<int32_t>(m_data);
    return (int64_t)val;
  }

  if (holds<uint32_t>()) {
    uint32_t val = std::get<uint32_t>(m_data);
    return (int64_t)val;
  }

  if (holds<double>()) {
    double val = std::get<double>(m_data);
    if (val >= -9223372036854775808.0 && val <= 9223372036854775807.0) {
      return (int64_t)val;
    }
  }

  return nullopt;
}

template <>
optional<double> Number::optional() const {
  if (holds<double>()) {
    return std::get<double>(m_data);
  }

  if (holds<int32_t>()) {
    return (double)std::get<int32_t>(m_data);
  }

  if (holds<uint32_t>()) {
    return (double)std::get<uint32_t>(m_data);
  }

  return (double)std::get<int64_t>(m_data);
}

bool Number::operator==(const Number &other) const {
  if (holds<double>() != other.holds<double>()) {
    return false;
  } else if (holds<double>()) {
    const double f = fabs(exactly<double>() - other.exactly<double>());
    return f <= numeric_limits<double>::epsilon() * f;
  } else {
    return optional<int64_t>() == other.optional<int64_t>();
  }
}

bool Number::operator!=(const Number &other) const {
  return !operator==(other);
}
