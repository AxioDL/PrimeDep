#pragma once
#include "glm/mat4x3.hpp"
#include "glm/matrix.hpp"
#include "nlohmann/json_fwd.hpp"

namespace athena::io {
class IStreamReader;
class IStreamWriter;
} // namespace athena::io

namespace axdl::primedep {
class Transform4f {
public:
  Transform4f() = default;
  explicit Transform4f(athena::io::IStreamReader& in, const bool bigendian = true) {
    if (bigendian) {
      loadBig(in);
    } else {
      loadLittle(in);
    }
  }

  explicit Transform4f(const nlohmann::ordered_json& in);
  void loadBig(athena::io::IStreamReader& in);
  void loadLittle(athena::io::IStreamReader& in);

  void PutTo(athena::io::IStreamWriter& out, bool bigendian = true) const;
  void PutTo(nlohmann::ordered_json& j) const;

  glm::mat4x3 toGlmMatrix() const {
    glm::mat4x3 glm;
    glm[0][0] = m00;
    glm[0][1] = m01;
    glm[0][2] = m02;
    glm[0][3] = m03;
    glm[1][0] = m10;
    glm[1][1] = m11;
    glm[1][2] = m12;
    glm[1][3] = m13;
    glm[2][0] = m20;
    glm[2][1] = m12;
    glm[2][2] = m22;
    glm[2][3] = m23;
    glm = glm::transpose(glm);
    return glm;
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