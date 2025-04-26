#pragma once

namespace athena::io {
class IStreamReader;
}

namespace axdl::primedep {
class Vector3f {
public:
  Vector3f() = default;
  Vector3f(const float x, const float y, const float z) : m_x(x), m_y(y), m_z(z) {}
  void loadBig(athena::io::IStreamReader& in);
  void loadLittle(athena::io::IStreamReader& in);

  template <bool BigEndian = true>
  static Vector3f Load(athena::io::IStreamReader& in) {
    Vector3f ret;
    if constexpr (BigEndian) {
      ret.loadBig(in);
    } else {
      ret.loadLittle(in);
    }
    return ret;
  }

private:
  float m_x{};
  float m_y{};
  float m_z{};
};
} // namespace axdl::primedep