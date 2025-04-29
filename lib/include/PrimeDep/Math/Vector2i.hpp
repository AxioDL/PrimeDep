#pragma once
#include <nlohmann/json_fwd.hpp>

namespace athena::io {
class IStreamReader;
class IStreamWriter;
} // namespace athena::io
namespace axdl::primedep {
class Vector2i {
public:
  Vector2i() = default;
  Vector2i(const int x, const int y) : m_x(x), m_y(y) {}
  explicit Vector2i(athena::io::IStreamReader& in, bool bigendian = true);
  explicit Vector2i(const nlohmann::ordered_json& in);

  void loadBig(athena::io::IStreamReader& in);
  void loadLittle(athena::io::IStreamReader& in);

  void PutTo(athena::io::IStreamWriter& out, bool bigendian = true) const;
  void PutTo(nlohmann::ordered_json& out) const;

  int& xr() { return m_x; }
  int x() const { return m_x; }
  void setX(const int x) { m_x = x; }

  int& yr() { return m_y; }
  int y() const { return m_y; }
  void setY(const int y) { m_y = y; }

private:
  int m_x{};
  int m_y{};
};
} // namespace axdl::primedep