#pragma once
#include <nlohmann/json_fwd.hpp>

namespace athena::io {
class IStreamReader;
class IStreamWriter;
} // namespace athena::io

namespace axdl::primedep {
class Vector3f {
public:
  Vector3f() = default;
  Vector3f(const float x, const float y, const float z) : m_x(x), m_y(y), m_z(z) {}
  explicit Vector3f(athena::io::IStreamReader& in, bool bigendian = true);
  explicit Vector3f(const nlohmann::ordered_json& in);
  void loadBig(athena::io::IStreamReader& in);
  void loadLittle(athena::io::IStreamReader& in);

  void PutTo(athena::io::IStreamWriter& out, bool bigendian = true) const;
  void PutTo(nlohmann::ordered_json& j) const;

  const float x() const { return m_x; }
  const float y() const { return m_y; }
  const float z() const { return m_z; }

private:
  float m_x{};
  float m_y{};
  float m_z{};
};
} // namespace axdl::primedep