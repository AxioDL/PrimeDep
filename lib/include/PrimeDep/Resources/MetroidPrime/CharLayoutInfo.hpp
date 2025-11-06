#pragma once

#include "PrimeDep/IResource.hpp"
#include "PrimeDep/Math/Vector3f.hpp"
#include "glm/fwd.hpp"

#include <tiny_gltf.h>

namespace axdl::primedep ::MetroidPrime {
class CharLayoutInfo final
: public TypedResource('CINF', ".skeleton.gltf", ".cin", DESCRIPTION("Character Skeletal Rigging")) {
public:
  struct Bone {
    uint32_t segId{};
    uint32_t parent{};
    Vector3f position;
    std::vector<uint32_t> children;

    Bone() = default;
    explicit Bone(athena::io::IStreamReader& in);

    void PutTo(athena::io::IStreamWriter& out) const;
  };

  struct BoneName {
    std::string name;
    uint32_t segId{};
    explicit BoneName(athena::io::IStreamReader& in);
    void PutTo(athena::io::IStreamWriter& out) const;
  };

  CharLayoutInfo(const char* ptr, std::size_t size);
  [[nodiscard]] bool writeCooked(std::string_view path) const override;
  [[nodiscard]] bool writeUncooked(std::string_view path) const override;

  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);

  static bool canIngest(const nlohmann::ordered_json& metadata) {
    return metadata["ResourceType"] == ResourceType().toString();
  }
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
    return nullptr;
  }

  [[nodiscard]] std::string boneName(uint32_t segId) const {
    auto it = std::ranges::find_if(m_boneNames, [&segId](const BoneName& bone) { return bone.segId == segId; });
    if (it != m_boneNames.end()) {
      return it->name;
    }
    return {};
  }

  Bone findBone(uint32_t segId) const {
    auto it = std::ranges::find_if(m_bones, [&segId](const Bone& b) { return b.segId == segId; });
    if (it != m_bones.end()) {
      return *it;
    }
    return {};
  }

private:
  void writeBone(tinygltf::Model & model, const Bone& bone, const glm::mat4& convMat) const;
  std::vector<Bone> m_bones;
  std::vector<uint32_t> m_sceneGraph;
  std::vector<BoneName> m_boneNames;
};
} // namespace axdl::primedep::MetroidPrime