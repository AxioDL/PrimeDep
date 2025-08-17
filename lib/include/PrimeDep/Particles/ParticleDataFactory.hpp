#pragma once
#include "ColorElement.hpp"
#include "PrimeDep/AssetId.hpp"
#include "PrimeDep/FourCC.hpp"

namespace axdl::primedep::particles {
class IntElement;
class RealElement;
class VectorElement;
class EmitterElement;
class ColorElement;
class UVElement;
class ModVectorElement;

namespace ParticleDataFactory {
FourCC GetClassID(athena::io::IStreamReader& reader);
bool GetBool(athena::io::IStreamReader& reader);
int GetInt(athena::io::IStreamReader& reader);
float GetReal(athena::io::IStreamReader& reader);
std::optional<AssetId32Big> GetAssetID32Big(athena::io::IStreamReader& reader, const FourCC& type);

ColorElement* GetColorElement(athena::io::IStreamReader& reader);
IntElement* GetIntElement(athena::io::IStreamReader& reader);
RealElement* GetRealElement(athena::io::IStreamReader& reader);
VectorElement* GetVectorElement(athena::io::IStreamReader& reader);
EmitterElement* GetEmitterElement(athena::io::IStreamReader& reader);
ModVectorElement* GetModVectorElement(athena::io::IStreamReader& reader);
UVElement* GetUVElement(athena::io::IStreamReader& reader);
}; // namespace ParticleDataFactory
}; // namespace axdl::primedep::particles