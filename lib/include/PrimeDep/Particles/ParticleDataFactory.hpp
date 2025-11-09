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
std::string GetClassID(const nlohmann::ordered_json& data);
void SetClassID(athena::io::IStreamWriter& writer, const FourCC& type);
void SetClassID(nlohmann::ordered_json& data, std::string_view type);

bool GetBool(athena::io::IStreamReader& reader);
bool GetBool(const nlohmann::ordered_json& reader, std::string_view type);
void SetBool(athena::io::IStreamWriter& writer, bool value);
void SetBool(nlohmann::ordered_json& writer, std::string_view type, bool value);

int GetInt(athena::io::IStreamReader& reader);
int GetInt(const nlohmann::ordered_json& reader, std::string_view type);
void SetInt(athena::io::IStreamWriter& writer, int value);
void SetInt(nlohmann::ordered_json& writer, std::string_view type, int value);

float GetReal(athena::io::IStreamReader& reader);
float GetReal(const nlohmann::ordered_json& reader, std::string_view type);
void SetReal(athena::io::IStreamWriter& writer, float value);
void SetReal(nlohmann::ordered_json& writer, std::string_view type, float value);

std::optional<AssetId32Big> GetAssetID32Big(athena::io::IStreamReader& reader, const FourCC& type);

ColorElement* GetColorElement(athena::io::IStreamReader& reader);
ColorElement* GetColorElement(const nlohmann::ordered_json& reader, std::string_view type);
IntElement* GetIntElement(athena::io::IStreamReader& reader);
IntElement* GetIntElement(const nlohmann::ordered_json& reader, std::string_view type);
RealElement* GetRealElement(athena::io::IStreamReader& reader);
RealElement* GetRealElement(const nlohmann::ordered_json& reader, std::string_view type);
VectorElement* GetVectorElement(athena::io::IStreamReader& reader);
VectorElement* GetVectorElement(const nlohmann::ordered_json& reader, std::string_view type);
EmitterElement* GetEmitterElement(athena::io::IStreamReader& reader);
EmitterElement* GetEmitterElement(const nlohmann::ordered_json& reader, std::string_view type);
ModVectorElement* GetModVectorElement(athena::io::IStreamReader& reader);
ModVectorElement* GetModVectorElement(const nlohmann::ordered_json& reader, std::string_view type);
UVElement* GetUVElement(athena::io::IStreamReader& reader);
UVElement* GetUVElement(const nlohmann::ordered_json& reader, std::string_view type);
}; // namespace ParticleDataFactory
}; // namespace axdl::primedep::particles