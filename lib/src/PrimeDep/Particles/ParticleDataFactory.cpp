#include "PrimeDep/Particles/ParticleDataFactory.hpp"

#include "PrimeDep/AssetId.hpp"
#include "PrimeDep/FourCC.hpp"
#include "PrimeDep/Particles/ColorElement.hpp"
#include "PrimeDep/Particles/EmitterElement.hpp"
#include "PrimeDep/Particles/IntElement.hpp"
#include "PrimeDep/Particles/ModVectorElement.hpp"
#include "PrimeDep/Particles/RealElement.hpp"
#include "PrimeDep/Particles/UVElement.hpp"
#include "PrimeDep/Particles/VectorElement.hpp"

namespace axdl::primedep::particles::ParticleDataFactory {
FourCC GetClassID(athena::io::IStreamReader& reader) { return FourCC(reader); }

bool GetBool(athena::io::IStreamReader& reader) {
  if (GetClassID(reader) != FOURCC('CNST')) {
    return false;
  }
  return reader.readBool();
}

int GetInt(athena::io::IStreamReader& reader) { return reader.readInt32Big(); }
float GetReal(athena::io::IStreamReader& reader) { return reader.readFloatBig(); }

std::optional<AssetId32Big> GetAssetID32Big(athena::io::IStreamReader& reader, const FourCC& type) {
  if (GetClassID(reader) == FOURCC('NONE')) {
    return {};
  }

  return {AssetId32Big(reader, type)};
}

ColorElement* GetColorElement(athena::io::IStreamReader& reader) { return nullptr; }
IntElement* GetIntElement(athena::io::IStreamReader& reader) { return nullptr; }
RealElement* GetRealElement(athena::io::IStreamReader& reader) { return nullptr; }
VectorElement* GetVectorElement(athena::io::IStreamReader& reader) { return nullptr; }
EmitterElement* GetEmitterElement(athena::io::IStreamReader& reader) { return nullptr; }
ModVectorElement* GetModVectorElement(athena::io::IStreamReader& reader) { return nullptr; }
UVElement* GetUVElement(athena::io::IStreamReader& reader) { return nullptr; }
} // namespace axdl::primedep::particles::ParticleDataFactory