#pragma once

#include "ObjectTag.hpp"
#include "PrimeDep/AssetId.hpp"
#include <map>
#include <nlohmann/json_fwd.hpp>

namespace axdl::primedep {
struct ResourceNameDatabase {
  static ResourceNameDatabase& instance() {
    static ResourceNameDatabase instance;
    return instance;
  }

  void load(std::string_view filename);

  bool hasPath(const ObjectTag32Big& tag) const { return assets.contains(tag.id()); }
  std::string pathForAsset(const ObjectTag32Big& tag) const;
  std::map<AssetId32Big, std::string> assets; // ID -> rep path
};
} // namespace axdl::primedep