#include "PrimeDep/Math/AABox.hpp"

namespace axdl::primedep {
void AABox::loadBig(athena::io::IStreamReader& in) {
  m_min = Vector3f(in, true);
  m_max = Vector3f(in, true);
}

void AABox::loadLittle(athena::io::IStreamReader& in) {
  m_min = Vector3f(in, false);
  m_max = Vector3f(in, false);
}

} // namespace axdl::primedep