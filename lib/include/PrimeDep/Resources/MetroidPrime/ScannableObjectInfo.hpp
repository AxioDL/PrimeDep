#pragma once

#include "PrimeDep/IResource.hpp"
#include "PrimeDep/Math/Vector2i.hpp"

namespace axdl::primedep::MetroidPrime {
class ScannableObjectInfo final
: public TypedResource('SCAN', ".scannable", ".scan", DESCRIPTION("Scannable Object Info")) {
public:
  static constexpr uint32_t kMaxBuckets = 4;
  static constexpr uint32_t kMagic = 0xBADBEEF;
  enum class ECategory {
    NoLog,
    SpacePirateData,
    ChozoLore,
    Creatures,
    Research,
    Artifacts,
  };

  enum class EImagePane {
    Invalid = -1,
    ImagePane0,
    ImagePane1,
    ImagePane2,
    ImagePane3,
    ImagePane01,
    ImagePane12,
    ImagePane23,
    ImagePane012,
    ImagePane123,
    ImagePane0123,
    ImagePane4,
    ImagePane5,
    ImagePane6,
    ImagePane7,
    ImagePane45,
    ImagePane56,
    ImagePane67,
    ImagePane456,
    ImagePane567,
    ImagePane4567,
  };
  struct Bucket {
    AssetId32Big texture{};
    float appearanceRange{};
    EImagePane imagePos{};
    Vector2i size{};
    float interval{};
    float fadeDuration{};

    Bucket() = default;
    explicit Bucket(athena::io::IStreamReader& in, uint32_t version);
    explicit Bucket(const nlohmann::json& in);

    void PutTo(athena::io::IStreamWriter& out, uint32_t version) const;
    void PutTo(nlohmann::ordered_json& out, uint32_t version) const;
  };

  ScannableObjectInfo(const char* ptr, std::size_t size);

  bool writeCooked(std::string_view path) const override;
  bool writeUncooked(std::string_view path) const override;
  static std::shared_ptr<IResource> loadCooked(const char* ptr, std::size_t size);
  static bool canIngest(const nlohmann::ordered_json& metadata);
  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path);

private:
  uint32_t m_version{};
  AssetId32Big m_frame{};
  AssetId32Big m_stringTable{};
  float m_scanSpeed{};
  uint32_t m_scanSpeedIdx{};
  ECategory m_category{};
  bool m_important{};
  std::array<Bucket, kMaxBuckets> m_buckets;
};
} // namespace axdl::primedep::MetroidPrime