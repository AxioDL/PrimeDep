#include "athena/IStreamReader.hpp"
#include "PrimeDep/Math/Vector3f.hpp"

namespace axdl::primedep {
void Vector3f::loadBig(athena::io::IStreamReader& in) {
  m_x = in.readFloatBig();
  m_y = in.readFloatBig();
  m_z = in.readFloatBig();
}

void Vector3f::loadLittle(athena::io::IStreamReader& in) {
  m_x = in.readFloatLittle();
  m_y = in.readFloatLittle();
  m_z = in.readFloatLittle();
}

} // namespace axdl::primedep