#include "PrimeDep/Resources/MetroidPrime/CharLayoutInfo.hpp"

#include <athena/MemoryReader.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <tiny_gltf.h>

namespace axdl::primedep::MetroidPrime {
CharLayoutInfo::Bone::Bone(athena::io::IStreamReader& in)
: segId(in.readUint32Big()), parent(in.readUint32Big()), position(in) {
  uint32_t childCount = in.readUint32Big();
  while (childCount--) {
    children.emplace_back(in.readUint32Big());
  }
}

void CharLayoutInfo::Bone::PutTo(athena::io::IStreamWriter& out) const {
  out.writeUint32Big(segId);
  out.writeUint32Big(parent);
  position.PutTo(out);
  out.writeUint32Big(children.size());
  for (const auto& child : children) {
    out.writeUint32Big(child);
  }
}

CharLayoutInfo::BoneName::BoneName(athena::io::IStreamReader& in) : name(in.readString()), segId(in.readUint32Big()) {}

void CharLayoutInfo::BoneName::PutTo(athena::io::IStreamWriter& out) const {
  out.writeString(name);
  out.writeUint32Big(segId);
}

CharLayoutInfo::CharLayoutInfo(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  uint32_t boneCount = in.readUint32Big();
  while (boneCount--) {
    m_bones.emplace_back(in);
  }
  uint32_t sceneGraphSize = in.readUint32Big();
  while (sceneGraphSize--) {
    m_sceneGraph.emplace_back(in.readUint32Big());
  }
  uint32_t boneNameCount = in.readUint32Big();
  while (boneNameCount--) {
    m_boneNames.emplace_back(in);
  }
}

static glm::mat4 zUpToYUpMatrix() {
  return glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}
static glm::mat4 yUpToZUpMatrix() {
  return glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}

bool CharLayoutInfo::writeCooked(const std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter out(p.generic_string());
  out.writeUint32Big(m_bones.size());
  for (const auto& bone : m_bones) {
    bone.PutTo(out);
  }

  out.writeUint32(m_sceneGraph.size());
  for (const auto& segId : m_sceneGraph) {
    out.writeUint32Big(segId);
  }

  out.writeUint32Big(m_boneNames.size());
  for (const auto& bone : m_boneNames) {
    bone.PutTo(out);
  }
  return !out.hasError();
}

void CharLayoutInfo::writeBone(tinygltf::Model& model, const Bone& bone, const glm::mat4& convMat) const {}

bool CharLayoutInfo::writeUncooked(std::string_view path) const { return false; }

std::shared_ptr<IResource> CharLayoutInfo::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<CharLayoutInfo>(ptr, size);
}
} // namespace axdl::primedep::MetroidPrime