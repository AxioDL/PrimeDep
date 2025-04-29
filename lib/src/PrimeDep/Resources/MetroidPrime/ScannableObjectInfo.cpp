#include "PrimeDep/Resources/MetroidPrime/ScannableObjectInfo.hpp"

#include "PrimeDep/ResourcePool.hpp"
#include "athena/MemoryReader.hpp"
#include "magic_enum/magic_enum.hpp"

namespace axdl::primedep::MetroidPrime {
ScannableObjectInfo::Bucket::Bucket(athena::io::IStreamReader& in, uint32_t version)
: texture(in, FOURCC('TXTR'))
, appearanceRange(in.readFloatBig())
, imagePos(magic_enum::enum_cast<EImagePane>(in.readInt32Big()).value_or(EImagePane::Invalid))
, size(version == 1 ? Vector2i() : Vector2i(in))
, interval(version == 1 ? 0.f : in.readFloatBig())
, fadeDuration(version < 3 ? 0.f : in.readFloatBig()) {}

ScannableObjectInfo::Bucket::Bucket(const nlohmann::json& in)
: texture(in)
, appearanceRange(in.value("AppearanceRange", 0.f))
, imagePos(magic_enum::enum_cast<EImagePane>(in.value("ImagePosition", "Invalid")).value_or(EImagePane::Invalid))
, size(in)
, interval(in.value("Interval", 0.f))
, fadeDuration(in.value("FadeDuration", 0.f)) {}

void ScannableObjectInfo::Bucket::PutTo(athena::io::IStreamWriter& out, const uint32_t version) const {
  texture.PutTo(out);
  out.writeFloatBig(appearanceRange);
  out.writeInt32Big(static_cast<int32_t>(imagePos));
  if (version != 1) {
    size.PutTo(out);
    out.writeFloatBig(interval);
  }
  if (version >= 3) {
    out.writeFloatBig(fadeDuration);
  }
}

void ScannableObjectInfo::Bucket::PutTo(nlohmann::ordered_json& out, const uint32_t version) const {
  texture.PutTo(out["Texture"]);
  out["AppearanceRange"] = appearanceRange;
  out["ImagePosition"] = magic_enum::enum_name(imagePos);
  out["FadeDuration"] = fadeDuration;
  if (version != 1) {
    size.PutTo(out["Size"]);
    out["Interval"] = interval;
  }
  if (version >= 3) {
    out["FadeDuration"] = fadeDuration;
  }
}

ScannableObjectInfo::ScannableObjectInfo(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);
  m_version = in.readUint32Big();
  const auto magic = in.readUint32Big();
  assert(magic == kMagic && "Unexpected magic value");
  m_frame = AssetId32Big(in, FOURCC('FRME'));
  m_stringTable = AssetId32Big(in, FOURCC('STRG'));

  if (m_version < 4) {
    m_scanSpeed = in.readFloatBig();
  } else {
    m_scanSpeedIdx = in.readUint32Big();
  }

  m_category = magic_enum::enum_cast<ECategory>(in.readInt32Big()).value();

  if (m_version > 4) {
    m_important = in.readBool();
  }

  for (int i = 0; i < 4; ++i) {
    m_buckets[i] = Bucket(in, m_version);
  }
}

bool ScannableObjectInfo::writeCooked(std::string_view path) const {
  const auto p = cookedPath(path);
  athena::io::FileWriter out(p.generic_string());
  out.writeUint32Big(m_version);
  out.writeUint32Big(kMagic);
  m_frame.PutTo(out);
  m_stringTable.PutTo(out);

  if (m_version < 4) {
    out.writeFloatBig(m_scanSpeed);
  } else {
    out.writeUint32Big(m_scanSpeedIdx);
  }

  out.writeUint32Big(static_cast<uint32_t>(m_category));

  if (m_version > 4) {
    out.writeBool(m_important);
  }

  for (const auto& bucket : m_buckets) {
    bucket.PutTo(out, m_version);
  }

  return !out.hasError();
}

bool ScannableObjectInfo::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);
  nlohmann::ordered_json j;
  j["Version"] = m_version;
  m_frame.PutTo(j["Frame"]);
  m_stringTable.PutTo(j["StringTable"]);
  if (m_version < 4) {
    j["ScanSpeedValue"] = m_scanSpeed;
  } else {
    j["ScanSpeedIndex"] = m_scanSpeedIdx;
  }

  j["Category"] = magic_enum::enum_name(m_category);

  if (m_version > 4) {
    j["Important"] = m_important;
  }

  auto& buckets = j["Buckets"];
  for (const auto& bucket : m_buckets) {
    bucket.PutTo(buckets.emplace_back(), m_version);
  }

  athena::io::FileWriter writer(p.generic_string());
  const auto js = j.dump(4) + "\n";
  writer.writeString(js, js.length());

  return !writer.hasError();
}

std::shared_ptr<IResource> ScannableObjectInfo::loadCooked(const char* ptr, std::size_t size) {
  return std::make_shared<ScannableObjectInfo>(ptr, size);
}

bool ScannableObjectInfo::canIngest(const nlohmann::ordered_json& metadata) {
  return metadata["ResourceType"] == ResourceType().toString();
}

std::shared_ptr<IResource> ScannableObjectInfo::ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
  return nullptr;
}
} // namespace axdl::primedep::MetroidPrime