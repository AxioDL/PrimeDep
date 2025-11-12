#include "PrimeDep/WorldCommon/MetroidPrime/MappableObject.hpp"

#include "athena/IStreamWriter.hpp"
#include "magic_enum/magic_enum.hpp"
#include <athena/IStreamReader.hpp>
#include <nlohmann/json.hpp>

namespace axdl::primedep::MetroidPrime {
MappableObject::MappableObject(athena::io::IStreamReader& in)
: m_type(static_cast<EMappableObjectType>(in.readUint32Big()))
, m_visMode(static_cast<EVisMode>(in.readUint32Big()))
, m_editorId(in.readUint32Big())
, m_unused(in.readUint32Big())
, m_transform(Transform4f(in)) {
  // Seek past padding
  in.seek(sizeof(uint32_t) * 4);
}

MappableObject::MappableObject(const nlohmann::ordered_json& in)
: m_type(magic_enum::enum_cast<EMappableObjectType>(in.value("Type", ""), magic_enum::case_insensitive)
             .value_or(EMappableObjectType::Invalid))
, m_visMode(magic_enum::enum_cast<EVisMode>(in.value("VisMode", ""), magic_enum::case_insensitive)
                .value_or(EVisMode::Invalid))
, m_editorId(in.value("EditorId", kInvalidEditorId.id))
, m_unused(in.value("Unused", 0))
, m_transform(in.value("Transform", nlohmann::ordered_json::object())) {}

void MappableObject::PutTo(athena::io::IStreamWriter& out) {
  out.writeUint32Big(static_cast<uint32_t>(m_type));
  out.writeUint32Big(static_cast<uint32_t>(m_visMode));
  out.writeUint32Big(m_editorId.id);
  out.writeUint32Big(m_unused);
  m_transform.PutTo(out);

  for (int i = 0; i < 4; ++i) {
    out.writeInt32Big(-1);
  }
}

void MappableObject::PutTo(nlohmann::ordered_json& out) const {
  out["Type"] = magic_enum::enum_name(m_type);
  out["VisMode"] = magic_enum::enum_name(m_visMode);
  out["EditorID"] = m_editorId.id;
  out["Unused"] = m_unused;
  m_transform.PutTo(out["Transform"]);
}

} // namespace axdl::primedep::MetroidPrime