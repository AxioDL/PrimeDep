#pragma once

#include "nlohmann/json_fwd.hpp"

#include <athena/IStreamReader.hpp>

namespace axdl::primedep {
class CharAnimTime {
public:
  enum class EType {
    NonZero,
    ZeroIncreasing,
    ZeroSteady,
    ZeroDecreasing,
    Infinity,
  };

  constexpr CharAnimTime() = default;
  explicit constexpr CharAnimTime(const float time)
  : m_time(time), m_type(m_time != 0.f ? EType::NonZero : EType::ZeroSteady) {}
  constexpr CharAnimTime(const EType type, const float t) : m_time(t), m_type(type) {}
  explicit CharAnimTime(athena::io::IStreamReader& in)
  : m_time(in.readFloatBig()), m_type(static_cast<EType>(in.readFloatBig())) {}

  static constexpr CharAnimTime Infinity() { return {EType::Infinity, 1.0f}; }
  [[nodiscard]] float GetSeconds() const { return m_time; }

  [[nodiscard]] bool EqualsZero() const;
  [[nodiscard]] bool EpsilonZero() const;
  [[nodiscard]] bool GreaterThanZero() const;
  bool operator==(const CharAnimTime& other) const;
  bool operator!=(const CharAnimTime& other) const;
  bool operator>=(const CharAnimTime& other) const;
  bool operator<=(const CharAnimTime& other) const;
  bool operator>(const CharAnimTime& other) const;
  bool operator<(const CharAnimTime& other) const;
  CharAnimTime& operator*=(const CharAnimTime& other);
  CharAnimTime& operator+=(const CharAnimTime& other);
  CharAnimTime operator+(const CharAnimTime& other) const;
  CharAnimTime& operator-=(const CharAnimTime& other);
  CharAnimTime operator-(const CharAnimTime& other) const;
  CharAnimTime operator*(const CharAnimTime& other) const;
  CharAnimTime operator*(const float& other) const;
  float operator/(const CharAnimTime& other) const;

  void PutTo(nlohmann::ordered_json& json) const;

private:
  float m_time = 0.f;
  EType m_type = EType::ZeroSteady;
};
} // namespace axdl::primedep