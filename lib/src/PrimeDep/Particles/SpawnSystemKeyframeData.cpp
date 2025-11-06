#include "PrimeDep/Particles/SpawnSystemKeyframeData.hpp"

#include "nlohmann/json.hpp"

namespace axdl::primedep::particles {

SpawnSystemKeyframeData::SpawnSystemKeyframeInfo::SpawnSystemKeyframeInfo(athena::io::IStreamReader& in)
: m_particleSystemId(in, FOURCC('PART'))
, m_unknown1(in.readUint32Big())
, m_unknown2(in.readUint32Big())
, m_unknown3(in.readUint32Big()) {}

SpawnSystemKeyframeData::SpawnSystemKeyframeInfo::SpawnSystemKeyframeInfo(const nlohmann::ordered_json& in)
: m_particleSystemId(in["ParticleSystem"], FOURCC('PART'))
, m_unknown1(in.value("Unknown1", 0))
, m_unknown2(in.value("Unknown2", 0))
, m_unknown3(in.value("Unknown3", 0)) {}

void SpawnSystemKeyframeData::SpawnSystemKeyframeInfo::PutTo(athena::io::IStreamWriter& out) const {
  m_particleSystemId.PutTo(out);
  out.writeUint32Big(m_unknown1);
  out.writeUint32Big(m_unknown2);
  out.writeUint32Big(m_unknown3);
}

void SpawnSystemKeyframeData::SpawnSystemKeyframeInfo::PutTo(nlohmann::ordered_json& out) const {
  m_particleSystemId.PutTo(out["ParticleSystem"], FOURCC('PART'));
  out["Unknown1"] = m_unknown1;
  out["Unknown2"] = m_unknown2;
  out["Unknown3"] = m_unknown3;
}

SpawnSystemKeyframeData::SpawnSystemKeyframeData(athena::io::IStreamReader& in)
: m_unknown1(in.readUint32Big())
, m_unknown2(in.readUint32Big())
, m_endFrame(in.readUint32Big())
, m_unknown3(in.readUint32Big()) {
  const uint32_t count = in.readUint32Big();
  for (int i = 0; i < count; ++i) {
    const uint32_t frame = in.readUint32Big();
    std::vector<SpawnSystemKeyframeInfo>& spawnInfo =
        m_frames.emplace_back(frame, std::vector<SpawnSystemKeyframeInfo>()).second;
    const uint32_t infoCount = in.readUint32Big();
    for (int j = 0; j < infoCount; ++j) {
      spawnInfo.emplace_back(in);
    }
  }
}

SpawnSystemKeyframeData::SpawnSystemKeyframeData(const nlohmann::ordered_json& in)
: m_unknown1(in.value("Unknown1", 0))
, m_unknown2(in.value("Unknown2", 0))
, m_endFrame(in.value("EndFrame", 0))
, m_unknown3(in.value("Unknown3", 0)) {
  for (const auto frames = in["Frames"]; const auto frame : frames) {
    int frameIndex = frame.value("Frame", 0);
    auto& spawns = m_frames.emplace_back(frameIndex, std::vector<SpawnSystemKeyframeInfo>()).second;
    for (const auto frameSpawns = frame["Spawns"]; const auto spawn : frameSpawns) {
      spawns.emplace_back(spawn);
    }
  }
}

void SpawnSystemKeyframeData::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32Big(m_unknown1);
  out.writeUint32Big(m_unknown2);
  out.writeUint32Big(m_endFrame);
  out.writeUint32Big(m_unknown3);
  out.writeUint32Big(m_frames.size());
  for (const auto [frame, spawns] : m_frames) {
    out.writeUint32Big(frame);
    for (const auto spawn : spawns) {
      spawn.PutTo(out);
    }
  }
}
void SpawnSystemKeyframeData::PutTo(nlohmann::ordered_json& out) const {
  out["Unknown1"] = m_unknown1;
  out["Unknown2"] = m_unknown2;
  out["EndFrame"] = m_endFrame;
  out["Unknown3"] = m_unknown3;
  auto& frames = out["Frames"];
  for (const auto [frame, spawns] : m_frames) {
    auto& frameOut = frames.emplace_back();
    frameOut["Frame"] = frame;
    auto& spawnsOut = frameOut["Spawns"];
    for (const auto spawn : spawns) {
      spawn.PutTo(spawnsOut.emplace_back());
    }
  }
}

} // namespace axdl::primedep::particles