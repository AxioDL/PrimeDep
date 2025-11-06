#include "PrimeDep/Animation/ParticlePOINode.hpp"

namespace axdl::primedep {

ParticlePOINode::ParticlePOINode() : POINode("root"sv, EPOIType::Particle, CharAnimTime(), -1, false, 1.f, -1, 0) {}

ParticlePOINode::ParticlePOINode(athena::io::IStreamReader& in) : POINode(in), x38_data(in) {}

ParticlePOINode::ParticlePOINode(const nlohmann::ordered_json& in) : POINode(in) {
  if (in.contains("Value")) {
    x38_data = ParticleData(in["Value"]);
  }
}

ParticlePOINode ParticlePOINode::CopyNodeMinusStartTime(const ParticlePOINode& node, const CharAnimTime& startTime) {
  ParticlePOINode ret = node;
  ret.x1c_time -= startTime;
  return ret;
}

void ParticlePOINode::PutTo(athena::io::IStreamWriter& out) const {
  POINode::PutTo(out);
  x38_data.PutTo(out);
}
void ParticlePOINode::PutTo(nlohmann::ordered_json& j) const {
  POINode::PutTo(j);
  x38_data.PutTo(j["Value"]);
}

} // namespace axdl::primedep
