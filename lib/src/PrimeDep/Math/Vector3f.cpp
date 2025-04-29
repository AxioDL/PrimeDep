#include "PrimeDep/Math/Vector3f.hpp"

#include <athena/IStreamReader.hpp>
#include <athena/IStreamWriter.hpp>
#include <nlohmann/json.hpp>

namespace axdl::primedep {
Vector3f::Vector3f(athena::io::IStreamReader& in, const bool bigendian) {
  if (bigendian) {
    loadBig(in);
  } else {
    loadLittle(in);
  }
}

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

void Vector3f::PutTo(athena::io::IStreamWriter& out, const bool bigendian) const {
  if (bigendian) {
    out.writeFloatBig(m_x);
    out.writeFloatBig(m_y);
    out.writeFloatBig(m_z);
  } else {
    out.writeFloatLittle(m_x);
    out.writeFloatLittle(m_y);
    out.writeFloatLittle(m_z);
  }
}

void Vector3f::PutTo(nlohmann::ordered_json& j) const {
  j["X"] = m_x;
  j["Y"] = m_y;
  j["Z"] = m_z;
}

} // namespace axdl::primedep