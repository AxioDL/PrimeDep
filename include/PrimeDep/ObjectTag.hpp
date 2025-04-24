#pragma once
#include "PrimeDep/FourCC.hpp"
#include "PrimeDep/AssetId.hpp"

namespace axdl::primedep {
template <typename T, bool BigEndian>
struct ObjectTag {
  FourCC type;
  AssetId<T, BigEndian> id;
};

using ObjectTag32Big = ObjectTag<uint32_t, true>;
using ObjectTag32Little = ObjectTag<uint32_t, false>;
using ObjectTag64Big = ObjectTag<uint64_t, true>;
using ObjectTag64Little = ObjectTag<uint64_t, false>;

} // namespace axdl::primedep
