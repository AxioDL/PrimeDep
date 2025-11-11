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

bool GetBool(athena::io::IStreamReader& reader, bool defaultValue = false);
bool GetBool(const nlohmann::ordered_json& reader, std::string_view propertyName, bool defaultValue = false);
void SetBool(athena::io::IStreamWriter& writer, bool value);
void SetBool(nlohmann::ordered_json& writer, std::string_view propertyName, bool value);

int GetInt(athena::io::IStreamReader& reader);
int GetInt(const nlohmann::ordered_json& reader, std::string_view propertyName);
void SetInt(athena::io::IStreamWriter& writer, int value);
void SetInt(nlohmann::ordered_json& writer, std::string_view propertyName, int value);

float GetReal(athena::io::IStreamReader& reader);
float GetReal(const nlohmann::ordered_json& reader, std::string_view propertyName);
void SetReal(athena::io::IStreamWriter& writer, float value);
void SetReal(nlohmann::ordered_json& writer, std::string_view propertyName, float value);

std::optional<AssetId32Big> GetAssetID32Big(athena::io::IStreamReader& reader, const FourCC& type);
std::optional<AssetId32Big> GetAssetID32Big(const nlohmann::ordered_json& reader, std::string_view propertyName,
                                            const FourCC& type);
void SetAssetID32Big(athena::io::IStreamWriter& writer, const std::optional<AssetId32Big>& value);
void SetAssetID32Big(nlohmann::ordered_json& writer, std::string_view propertyName,
                     const std::optional<AssetId32Big>& value, const FourCC& type);

ColorElement* GetColorElement(athena::io::IStreamReader& reader, std::string_view propertyName);
ColorElement* GetColorElement(const nlohmann::ordered_json& reader, std::string_view propertyName);
IntElement* GetIntElement(athena::io::IStreamReader& reader, std::string_view propertyName);
IntElement* GetIntElement(const nlohmann::ordered_json& reader, std::string_view propertyName);
RealElement* GetRealElement(athena::io::IStreamReader& reader, std::string_view propertyName);
RealElement* GetRealElement(const nlohmann::ordered_json& reader, std::string_view propertyName);
VectorElement* GetVectorElement(athena::io::IStreamReader& reader, std::string_view propertyName);
VectorElement* GetVectorElement(const nlohmann::ordered_json& reader, std::string_view propertyName);
EmitterElement* GetEmitterElement(athena::io::IStreamReader& reader, std::string_view propertyName);
EmitterElement* GetEmitterElement(const nlohmann::ordered_json& reader, std::string_view propertyName);
ModVectorElement* GetModVectorElement(athena::io::IStreamReader& reader, std::string_view propertyName);
ModVectorElement* GetModVectorElement(const nlohmann::ordered_json& reader, std::string_view propertyName);
UVElement* GetUVElement(athena::io::IStreamReader& reader, std::string_view propertyName);
UVElement* GetUVElement(const nlohmann::ordered_json& reader, std::string_view propertyName);

template <class ElementType>
ElementType* GetElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  return nullptr;
}
template <class ElementType>
ElementType* GetElement(const nlohmann::ordered_json& reader, std::string_view propertyName) {
  return nullptr;
}

template <>
inline ColorElement* GetElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  return GetColorElement(reader, propertyName);
}
template <>
inline ColorElement* GetElement(const nlohmann::ordered_json& reader, std::string_view propertyName) {
  return GetColorElement(reader, propertyName);
}

template <>
inline IntElement* GetElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  return GetIntElement(reader, propertyName);
}
template <>
inline IntElement* GetElement(const nlohmann::ordered_json& reader, std::string_view propertyName) {
  return GetIntElement(reader, propertyName);
}

template <>
inline RealElement* GetElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  return GetRealElement(reader, propertyName);
}
template <>
inline RealElement* GetElement(const nlohmann::ordered_json& reader, std::string_view propertyName) {
  return GetRealElement(reader, propertyName);
}

template <>
inline VectorElement* GetElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  return GetVectorElement(reader, propertyName);
}
template <>
inline VectorElement* GetElement(const nlohmann::ordered_json& reader, std::string_view propertyName) {
  return GetVectorElement(reader, propertyName);
}
template <>
inline EmitterElement* GetElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  return GetEmitterElement(reader, propertyName);
}
template <>
inline EmitterElement* GetElement(const nlohmann::ordered_json& reader, std::string_view propertyName) {
  return GetEmitterElement(reader, propertyName);
}
template <>
inline ModVectorElement* GetElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  return GetModVectorElement(reader, propertyName);
}
template <>
inline ModVectorElement* GetElement(const nlohmann::ordered_json& reader, std::string_view propertyName) {
  return GetModVectorElement(reader, propertyName);
}
template <>
inline UVElement* GetElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  return GetUVElement(reader, propertyName);
}
template <>
inline UVElement* GetElement(const nlohmann::ordered_json& reader, std::string_view propertyName) {
  return GetUVElement(reader, propertyName);
}
}; // namespace ParticleDataFactory
}; // namespace axdl::primedep::particles