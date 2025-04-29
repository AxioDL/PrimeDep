#include "PrimeDep/Math/Vector2i.hpp"
#include <athena/IStreamReader.hpp>
#include <athena/IStreamWriter.hpp>
#include <nlohmann/json.hpp>

namespace axdl::primedep {
Vector2i::Vector2i(athena::io::IStreamReader& in, const bool bigendian) {
  if (bigendian) {
    loadBig(in);
  } else {
    loadLittle(in);
  }
}
Vector2i::Vector2i(const nlohmann::ordered_json& in) : m_x(in.value("X", 0)), m_y(in.value("Y", 0)) {}

void Vector2i::loadBig(athena::io::IStreamReader& in) {
  m_x = in.readInt32Big();
  m_y = in.readInt32Big();
}
void Vector2i::loadLittle(athena::io::IStreamReader& in) {
  m_x = in.readInt32Little();
  m_y = in.readInt32Little();
}

void Vector2i::PutTo(athena::io::IStreamWriter& out, const bool bigendian) const {
  if (bigendian) {
    out.writeInt32Big(m_x);
    out.writeInt32Big(m_y);
  } else {
    out.writeInt32Little(m_x);
    out.writeInt32Little(m_y);
  }
}

void Vector2i::PutTo(nlohmann::ordered_json& out) const {
  out["X"] = m_x;
  out["Y"] = m_y;
}

} // namespace axdl::primedep