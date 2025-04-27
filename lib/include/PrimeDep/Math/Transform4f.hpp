#pragma once
#include "nlohmann/json_fwd.hpp"

namespace athena::io {
class IStreamReader;
}

namespace axdl::primedep {
class Transform4f {
public:
  void loadBig(athena::io::IStreamReader& in);
  void loadLittle(athena::io::IStreamReader& in);

  /**
   * Loads an @see Transform4f from the current location in the specified buffer.
   * Byte swapping where necessary.
   * @tparam BigEndian Whether to load float values in as big endian or little, true for big, false for little
   * @param in Buffer to read from
   * @return Transform4f at the current position in the buffer
   */
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

  void PutTo(nlohmann::ordered_json& j) const;

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