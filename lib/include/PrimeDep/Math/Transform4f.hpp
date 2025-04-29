#pragma once
#include "nlohmann/json_fwd.hpp"

namespace athena::io {
class IStreamReader;
class IStreamWriter;
} // namespace athena::io

namespace axdl::primedep {
class Transform4f {
public:
  Transform4f(athena::io::IStreamReader& in, const bool bigendian = true) {
    if (bigendian) {
      loadBig(in);
    } else {
      loadLittle(in);
    }
  }
  void loadBig(athena::io::IStreamReader& in);
  void loadLittle(athena::io::IStreamReader& in);

  void PutTo(athena::io::IStreamWriter& out, bool bigendian = true) const;
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