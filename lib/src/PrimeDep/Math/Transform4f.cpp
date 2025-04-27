#include "PrimeDep/Math/Transform4f.hpp"

#include "PrimeDep/Math/Vector3f.hpp"
#include "athena/IStreamReader.hpp"

#include <nlohmann/json.hpp>

namespace axdl::primedep {
void Transform4f::loadBig(athena::io::IStreamReader& in) {
  m00 = in.readFloatBig();
  m01 = in.readFloatBig();
  m02 = in.readFloatBig();
  m03 = in.readFloatBig();
  m10 = in.readFloatBig();
  m11 = in.readFloatBig();
  m12 = in.readFloatBig();
  m13 = in.readFloatBig();
  m20 = in.readFloatBig();
  m21 = in.readFloatBig();
  m22 = in.readFloatBig();
  m23 = in.readFloatBig();
}

void Transform4f::loadLittle(athena::io::IStreamReader& in) {
  m00 = in.readFloatLittle();
  m01 = in.readFloatLittle();
  m02 = in.readFloatLittle();
  m03 = in.readFloatLittle();
  m10 = in.readFloatLittle();
  m11 = in.readFloatLittle();
  m12 = in.readFloatLittle();
  m13 = in.readFloatLittle();
  m20 = in.readFloatLittle();
  m21 = in.readFloatLittle();
  m22 = in.readFloatLittle();
  m23 = in.readFloatLittle();
}

void Transform4f::PutTo(nlohmann::ordered_json& j) const {
  Vector3f tmp(m00, m01, m02);
  tmp.PutTo(j["M0"]);
  tmp = Vector3f(m10, m11, m12);
  tmp.PutTo(j["M1"]);
  tmp = Vector3f(m20, m21, m22);
  tmp.PutTo(j["M2"]);
  tmp = Vector3f(m03, m13, m23);
  tmp.PutTo(j["Pos"]);
}

} // namespace axdl::primedep