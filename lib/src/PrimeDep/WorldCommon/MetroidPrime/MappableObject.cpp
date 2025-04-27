#include "PrimeDep/WorldCommon/MetroidPrime/MappableObject.hpp"

#include "magic_enum/magic_enum.hpp"
#include <athena/IStreamReader.hpp>
#include <nlohmann/json.hpp>

namespace axdl::primedep::MetroidPrime {
MappableObject::MappableObject(athena::io::IStreamReader& in)
: m_type(static_cast<EMappableObjectType>(in.readUint32Big()))
, m_visMode(static_cast<EVisMode>(in.readUint32Big()))
, m_editorId(in.readUint32Big())
, m_unused(in.readUint32Big())
, m_transform(Transform4f::Load<true>(in)) {
  // Seek past padding
  in.seek(sizeof(uint32_t) * 4);
}

void MappableObject::PutTo(nlohmann::ordered_json& j) const {
  j["Type"] = magic_enum::enum_name(m_type);
  j["VisMode"] = magic_enum::enum_name(m_visMode);
  j["EditorID"] = m_editorId.id;
  m_transform.PutTo(j["Transform"]);
}

} // namespace axdl::primedep::MetroidPrime