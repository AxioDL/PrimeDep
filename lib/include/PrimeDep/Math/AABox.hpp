#pragma once
#include "PrimeDep/Math/Vector3f.hpp"

#include <cfloat>

namespace athena::io {
class IStreamReader;
}

namespace axdl::primedep {
class AABox {
public:
  AABox() = default;
  void loadBig(athena::io::IStreamReader& in);
  void loadLittle(athena::io::IStreamReader& in);

  template<bool BigEndian = true>
  static AABox Load(athena::io::IStreamReader& in) {
    AABox ret;
    if constexpr (BigEndian) {
      ret.loadBig(in);
    } else {
      ret.loadLittle(in);
    }
    return ret;
  }

private:
  Vector3f m_min{FLT_MAX, FLT_MAX, FLT_MAX};
  Vector3f m_max{FLT_MIN, FLT_MIN, FLT_MIN};
};
} // namespace axdl::primedep