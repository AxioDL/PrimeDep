#pragma once
#include "PrimeDep/Math/Vector3f.hpp"
#include "nlohmann/json.hpp"

#include <cfloat>

namespace athena::io {
class IStreamReader;
}

namespace axdl::primedep {
class AABox {
public:
  AABox() = default;
  explicit AABox(athena::io::IStreamReader& in, const bool bigendian = true) {
    if (bigendian) {
      loadBig(in);
    } else {
      loadLittle(in);
    }
  }

  void loadBig(athena::io::IStreamReader& in);
  void loadLittle(athena::io::IStreamReader& in);

  void PutTo(nlohmann::ordered_json& j) const {
    m_min.PutTo(j["Min"]);
    m_max.PutTo(j["Max"]);
  }

private:
  Vector3f m_min{FLT_MAX, FLT_MAX, FLT_MAX};
  Vector3f m_max{FLT_MIN, FLT_MIN, FLT_MIN};
};
} // namespace axdl::primedep