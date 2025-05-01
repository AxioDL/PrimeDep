#include "PrimeDep/Math/Color.hpp"

#include <athena/IStreamReader.hpp>
#include <athena/IStreamWriter.hpp>
#include <nlohmann/json.hpp>

namespace axdl::primedep {
Color::Color(athena::io::IStreamReader& in, const bool bigendian) {
  if (bigendian) {
    loadBig(in);
  } else {
    loadLittle(in);
  }
}

Color::Color(const nlohmann::ordered_json& in)
: m_r(in.value("R", 0.f)), m_g(in.value("G", 0.f)), m_b(in.value("B", 0.f)), m_a(in.value("A", 1.f)) {}

void Color::loadBig(athena::io::IStreamReader& in) {
  m_r = in.readFloatBig();
  m_g = in.readFloatBig();
  m_b = in.readFloatBig();
  m_a = in.readFloatBig();
}

void Color::loadLittle(athena::io::IStreamReader& in) {
  m_r = in.readFloatLittle();
  m_g = in.readFloatLittle();
  m_b = in.readFloatLittle();
  m_a = in.readFloatLittle();
}

void Color::PutTo(athena::io::IStreamWriter& out, const bool bigendian) const {
  if (bigendian) {
    out.writeFloatBig(m_r);
    out.writeFloatBig(m_g);
    out.writeFloatBig(m_b);
    out.writeFloatBig(m_a);
  } else {
    out.writeFloatLittle(m_r);
    out.writeFloatLittle(m_g);
    out.writeFloatLittle(m_b);
    out.writeFloatLittle(m_a);
  }
}

void Color::PutTo(nlohmann::ordered_json& j) const {
  j["R"] = m_r;
  j["G"] = m_g;
  j["B"] = m_b;
  j["A"] = m_a;
}

} // namespace axdl::primedep