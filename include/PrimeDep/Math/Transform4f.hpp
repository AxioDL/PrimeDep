#pragma once

namespace athena::io {
class IStreamReader;
}

namespace axdl::primedep {
class Transform4f {
public:
  void loadBig(athena::io::IStreamReader& in);
  void loadLittle(athena::io::IStreamReader& in);

  template <bool BigEndian = true>
  static Transform4f Load(athena::io::IStreamReader& in) noexcept {
    Transform4f ret;
    if constexpr (BigEndian) {
      ret.loadBig(in);
    } else {
      ret.loadLittle(in);
    }

    return ret;
  }

private:
  float m00{1.f};
  float m01{0.f};
  float m02{0.f};
  float m03{0.f};
  float m10{0.f};
  float m11{1.f};
  float m12{0.f};
  float m13{0.f};
  float m20{0.f};
  float m21{0.f};
  float m22{1.f};
  float m23{0.f};
};
} // namespace axdl::primedep