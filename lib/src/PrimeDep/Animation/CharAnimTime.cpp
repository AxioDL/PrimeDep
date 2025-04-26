#include "PrimeDep/Animation/CharAnimTime.hpp"

#include "magic_enum/magic_enum.hpp"
#include <nlohmann/json.hpp>

#include <cmath>

namespace axdl::primedep {
bool CharAnimTime::EqualsZero() const {
  if (m_type == EType::ZeroIncreasing || m_type == EType::ZeroSteady || m_type == EType::ZeroDecreasing)
    return true;

  return m_time == 0.f;
}

bool CharAnimTime::EpsilonZero() const { return std::fabs(m_time) < 0.00001f; }

bool CharAnimTime::GreaterThanZero() const {
  if (EqualsZero())
    return false;
  return m_time > 0.f;
}

bool CharAnimTime::operator==(const CharAnimTime& other) const {
  if (m_type == EType::NonZero) {
    if (other.m_type == EType::NonZero) {
      return m_time == other.m_time;
    }
    return false;
  }

  if (EqualsZero()) {
    if (other.EqualsZero()) {
      int type = -1;
      if (m_type != EType::ZeroDecreasing) {
        type = m_type == EType::ZeroSteady ? 0 : 1;
      }

      int otherType = -1;
      if (other.m_type != EType::ZeroDecreasing) {
        otherType = other.m_type == EType::ZeroSteady ? 0 : 1;
      }

      return type == otherType;
    }
    return false;
  }

  if (other.m_type == EType::Infinity) {
    return m_time * other.m_time > 0.f;
  }

  return false;
}

bool CharAnimTime::operator!=(const CharAnimTime& other) const { return !(*this == other); }

bool CharAnimTime::operator>=(const CharAnimTime& other) const {
  if (*this == other)
    return true;

  return *this > other;
}

bool CharAnimTime::operator<=(const CharAnimTime& other) const {
  if (*this == other)
    return true;

  return *this < other;
}

bool CharAnimTime::operator>(const CharAnimTime& other) const { return !(*this == other) && !(*this < other); }

bool CharAnimTime::operator<(const CharAnimTime& other) const {
  if (m_type == EType::NonZero) {
    if (other.m_type == EType::NonZero) {
      return m_time < other.m_time;
    }

    return other.EqualsZero() ? m_time < 0.f : other.m_time > 0;
  }

  if (!EqualsZero()) {
    if (other.m_type == EType::Infinity) {
      return m_time >= 0 || other.m_time <= 0.f;
    }

    return m_time < 0.f;
  }

  if (!other.EqualsZero()) {
    if (other.m_type == EType::NonZero) {
      return other.m_time > 0.f;
    }

    return other.m_time > 0.f;
  }

  const int type = m_type == EType::ZeroDecreasing ? -1 : m_type == EType::ZeroSteady ? 0 : 1;
  const int otherType = other.m_type == EType::ZeroDecreasing ? -1 : other.m_type == EType::ZeroSteady ? 0 : 1;

  return type < otherType;
}

CharAnimTime& CharAnimTime::operator*=(const CharAnimTime& other) { return *this = *this * other; }

CharAnimTime& CharAnimTime::operator+=(const CharAnimTime& other) { return *this = *this + other; }

CharAnimTime CharAnimTime::operator+(const CharAnimTime& other) const {
  if (m_type == EType::Infinity && other.m_type == EType::Infinity) {
    if (other.m_time != m_time) {
      return {};
    }
    return *this;
  }
  if (m_type == EType::Infinity) {
    return *this;
  }
  if (other.m_type == EType::Infinity) {
    return other;
  }

  if (!EqualsZero() || !other.EqualsZero()) {
    return CharAnimTime{m_time + other.m_time};
  }

  int type = -1;
  if (m_type != EType::ZeroDecreasing) {
    type = m_type == EType::ZeroSteady ? 0 : 1;
  }

  int otherType = -1;
  if (other.m_type != EType::ZeroDecreasing) {
    otherType = other.m_type == EType::ZeroSteady ? 0 : 1;
  }

  type += otherType;
  otherType = std::max(-1, std::min(type, 1));

  if (otherType == -1) {
    return {EType::ZeroDecreasing, 0.f};
  }
  if (otherType == 0) {
    return {EType::ZeroSteady, 0.f};
  }

  return {EType::ZeroIncreasing, 0.f};
}

CharAnimTime& CharAnimTime::operator-=(const CharAnimTime& other) { return *this = *this - other; }

CharAnimTime CharAnimTime::operator-(const CharAnimTime& other) const {
  if (m_type == EType::Infinity && other.m_type == EType::Infinity) {
    if (other.m_time == m_time)
      return {};
    return *this;
  }
  if (m_type == EType::Infinity) {
    return *this;
  }
  if (other.m_type == EType::Infinity) {
    return {EType::Infinity, -other.m_time};
  }

  if (!EqualsZero() || !other.EqualsZero()) {
    return CharAnimTime{m_time - other.m_time};
  }

  int type = -1;
  if (m_type != EType::ZeroDecreasing) {
    type = m_type == EType::ZeroSteady ? 0 : 1;
  }

  int otherType = -1;
  if (other.m_type != EType::ZeroDecreasing) {
    otherType = other.m_type == EType::ZeroSteady ? 0 : 1;
  }

  type -= otherType;
  if (type == -1) {
    return {EType::ZeroDecreasing, 0.f};
  }
  if (type == 0) {
    return {EType::ZeroSteady, 0.f};
  }

  return {EType::ZeroIncreasing, 0.f};
}

CharAnimTime CharAnimTime::operator*(const CharAnimTime& other) const {
  if (m_type == EType::Infinity && other.m_type == EType::Infinity) {
    if (other.m_time != m_time)
      return {};
    return *this;
  }
  if (m_type == EType::Infinity) {
    return *this;
  }
  if (other.m_type == EType::Infinity) {
    return other;
  }

  if (!EqualsZero() || !other.EqualsZero()) {
    return CharAnimTime{m_time * other.m_time};
  }

  int type = -1;
  if (m_type != EType::ZeroDecreasing) {
    type = m_type == EType::ZeroSteady ? 0 : 1;
  }

  int otherType = -1;
  if (other.m_type != EType::ZeroDecreasing) {
    otherType = other.m_type == EType::ZeroSteady ? 0 : 1;
  }

  type += otherType;
  otherType = std::max(-1, std::min(type, 1));

  if (otherType == -1) {
    return {EType::ZeroDecreasing, 0.f};
  }

  if (otherType == 0) {
    return {EType::ZeroSteady, 0.f};
  }

  return {EType::ZeroIncreasing, 0.f};
}

CharAnimTime CharAnimTime::operator*(const float& other) const {
  if (other == 0.f)
    return {};

  if (!EqualsZero())
    return CharAnimTime{m_time * other};

  if (other > 0.f)
    return *this;

  if (m_type == EType::ZeroDecreasing) {
    return {EType::ZeroIncreasing, 0.f};
  }
  if (m_type == EType::ZeroSteady) {
    return {EType::ZeroSteady, 0.f};
  }
  return {EType::ZeroDecreasing, 0.f};
}

float CharAnimTime::operator/(const CharAnimTime& other) const {
  if (other.EqualsZero()) {
    return 0.f;
  }

  return m_time / other.m_time;
}

void CharAnimTime::PutTo(nlohmann::ordered_json& json) const {
  json["Type"] = magic_enum::enum_name(m_type);
  json["Time"] = m_time;
}

} // namespace axdl::primedep