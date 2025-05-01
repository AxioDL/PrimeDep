#pragma once
#include <nlohmann/json_fwd.hpp>

namespace athena::io {
class IStreamReader;
class IStreamWriter;
} // namespace athena::io

namespace axdl::primedep {
class Color {
public:
  Color() = default;
  Color(const float r, const float g, const float b, const float a = 1.f) : m_r(r), m_g(g), m_b(b), m_a(a) {}
  explicit Color(athena::io::IStreamReader& in, bool bigendian = true);
  explicit Color(const nlohmann::ordered_json& in);

  void loadBig(athena::io::IStreamReader& in);
  void loadLittle(athena::io::IStreamReader& in);

  void PutTo(athena::io::IStreamWriter& out, bool bigendian = true) const;
  void PutTo(nlohmann::ordered_json& j) const;

  float r() const { return m_r; }
  float g() const { return m_g; }
  float b() const { return m_b; }
  float a() const { return m_a; }

private:
  float m_r{};
  float m_g{};
  float m_b{};
  float m_a{};
};
} // namespace axdl::primedep