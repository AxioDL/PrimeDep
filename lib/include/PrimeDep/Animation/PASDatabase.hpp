#pragma once

#include "PrimeDep/Resources/MetroidPrime/SkinnedModel.hpp"

#include <nlohmann/json_fwd.hpp>

namespace athena::io {
class IStreamReader;
class IStreamWriter;
} // namespace athena::io

namespace axdl::primedep::animations {
class PASDatabase {
public:
  explicit CharacterInfo(athena::io::IStreamReader& in);
  explicit CharacterInfo(const nlohmann::ordered_json& in);

  void PutTo(athena::io::IStreamWriter& out) const;
  void PutTo(nlohmann::ordered_json& out) const;

private:
};
} // namespace axdl::primedep::animations
