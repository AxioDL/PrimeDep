#include "PrimeDep/Math/Transform4f.hpp"

#include "athena/IStreamReader.hpp"

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

} // namespace axdl::primedep