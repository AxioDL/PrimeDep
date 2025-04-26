#include "PrimeDep/Math/AABox.hpp"

namespace axdl::primedep {
void AABox::loadBig(athena::io::IStreamReader& in) {
  m_min = Vector3f::Load<true>(in);
  m_max = Vector3f::Load<true>(in);
}

void AABox::loadLittle(athena::io::IStreamReader& in) {
  m_min = Vector3f::Load<false>(in);
  m_max = Vector3f::Load<false>(in);
}

} // namespace axdl::primedep