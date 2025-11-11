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

#include <nlohmann/json.hpp>

namespace axdl::primedep::particles::ParticleDataFactory {
FourCC GetClassID(athena::io::IStreamReader& reader) { return FourCC(reader); }
std::string GetClassID(const nlohmann::ordered_json& data) { return data.value("Type", std::string()); }

void SetClassID(athena::io::IStreamWriter& writer, const FourCC& type) { type.PutTo(writer); }
void SetClassID(nlohmann::ordered_json& data, const std::string_view type) { data["Type"] = type; }

bool GetBool(athena::io::IStreamReader& reader, const bool defaultValue) {
  if (GetClassID(reader) != FOURCC('CNST')) {
    return defaultValue;
  }
  return reader.readBool();
}
bool GetBool(const nlohmann::ordered_json& reader, const std::string_view propertyName, bool defaultValue) {
  if (propertyName.empty()) {
    return defaultValue;
  }
  return reader.value(propertyName, defaultValue);
}
void SetBool(athena::io::IStreamWriter& writer, const bool value) {
  SetClassID(writer, FOURCC('CNST'));
  writer.writeBool(value);
}
void SetBool(nlohmann::ordered_json& writer, const std::string_view propertyName, const bool value) {
  if (propertyName.empty()) {
    return;
  }
  writer[propertyName] = value;
}

int GetInt(athena::io::IStreamReader& reader) { return reader.readInt32Big(); }
int GetInt(const nlohmann::ordered_json& reader, const std::string_view propertyName) {
  if (propertyName.empty()) {
    return 0;
  }
  return reader.value(propertyName, 0);
}
void SetInt(athena::io::IStreamWriter& writer, const int value) { writer.writeInt32Big(value); }
void SetInt(nlohmann::ordered_json& writer, const std::string_view propertyName, const int value) {
  if (propertyName.empty()) {
    return;
  }

  writer[propertyName] = value;
}

float GetReal(athena::io::IStreamReader& reader) { return reader.readFloatBig(); }
float GetReal(const nlohmann::ordered_json& reader, const std::string_view propertyName) {
  return reader.value(propertyName, 0);
}
void SetReal(athena::io::IStreamWriter& writer, const float value) { writer.writeFloatBig(value); }
void SetReal(nlohmann::ordered_json& writer, const std::string_view propertyName, const float value) {
  if (propertyName.empty()) {
    return;
  }

  writer[propertyName] = value;
}

std::optional<AssetId32Big> GetAssetID32Big(athena::io::IStreamReader& reader, const FourCC& type) {
  if (GetClassID(reader) == FOURCC('NONE')) {
    return {kInvalidAssetId32Big};
  }

  return {AssetId32Big(reader, type)};
}

std::optional<AssetId32Big> GetAssetID32Big(const nlohmann::ordered_json& reader, const std::string_view propertyName,
                                            const FourCC& type) {
  if (!reader.contains(propertyName)) {
    return {kInvalidAssetId32Big};
  }

  return {AssetId32Big(reader, type)};
}

void SetAssetID32Big(athena::io::IStreamWriter& writer, const std::optional<AssetId32Big>& value) {
  if (!value || value == kInvalidAssetId32Big) {
    SetClassID(writer, FOURCC('NONE'));
    return;
  }
  value->PutTo(writer);
}

void SetAssetID32Big(nlohmann::ordered_json& writer, const std::string_view propertyName,
                     const std::optional<AssetId32Big>& value, const FourCC& type) {
  if (propertyName.empty()) {
    return;
  }
  value->PutTo(writer[propertyName], type);
}

ColorElement* GetColorElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  ColorElement* element = nullptr;

  switch (GetClassID(reader)) {
  case CENone::ClassID():
    element = new CENone(reader);
    break;
  case CEConstant::ClassID():
    element = new CEConstant(reader);
    break;
  case CEKeyframeEmitter::ClassIDNormal():
  case CEKeyframeEmitter::ClassIDPercent():
    element = new CEKeyframeEmitter(reader);
    break;
  case CETimeChain::ClassID():
    element = new CETimeChain(reader);
    break;
  case CEFadeEnd::ClassID():
    element = new CEFadeEnd(reader);
    break;
  case CEFade::ClassID():
    element = new CEFade(reader);
    break;
  case CEPulse::ClassID():
    element = new CEPulse(reader);
    break;
  case CEParticleColor::ClassID():
    element = new CEParticleColor(reader);
    break;
  default:
    break;
  }
  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

ColorElement* GetColorElement(const nlohmann::ordered_json& reader, const std::string_view propertyName) {
  ColorElement* element = nullptr;

  if (!reader.contains(propertyName)) {
    return nullptr;
  }
  const auto type = GetClassID(reader);
  if (type == CENone::ClassName()) {
    element = new CENone(reader);
  } else if (type == CEConstant::ClassName()) {
    element = new CEConstant(reader);
  } else if (type == CEKeyframeEmitter::ClassName()) {
    element = new CEKeyframeEmitter(reader);
  } else if (type == CETimeChain::ClassName()) {
    element = new CETimeChain(reader);
  } else if (type == CEFadeEnd::ClassName()) {
    element = new CEFadeEnd(reader);
  } else if (type == CEFade::ClassName()) {
    element = new CEFade(reader);
  } else if (type == CEPulse::ClassName()) {
    element = new CEPulse(reader);
  } else if (type == CEParticleColor::ClassName()) {
    element = new CEParticleColor(reader);
  }
  if (element) {
    element->setPropertyName(propertyName);
  }
  return element;
}

IntElement* GetIntElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  IntElement* element = nullptr;

  switch (GetClassID(reader)) {
  case IENone::ClassID():
    element = new IENone(reader);
    break;
  case IEConstant::ClassID():
    element = new IEConstant(reader);
    break;
  case IEKeyframeEmitter::ClassIDNormal():
  case IEKeyframeEmitter::ClassIDPercent():
    element = new IEKeyframeEmitter(reader);
    break;
  case IETimeScale::ClassID():
    element = new IETimeScale(reader);
    break;
  case IEDeath::ClassID():
    element = new IEDeath(reader);
    break;
  case IETimeChain::ClassID():
    element = new IETimeChain(reader);
    break;
  case IEAdd::ClassID():
    element = new IEAdd(reader);
    break;
  case IEMultiply::ClassID():
    element = new IEMultiply(reader);
    break;
  case IEModulo::ClassID():
    element = new IEModulo(reader);
    break;
  case IERandom::ClassID():
    element = new IERandom(reader);
    break;
  case IEImpulse::ClassID():
    element = new IEImpulse(reader);
    break;
  case IELifetimePercent::ClassID():
    element = new IELifetimePercent(reader);
    break;
  case IESampleAndHold::ClassID():
    element = new IESampleAndHold(reader);
    break;
  case IEInitialRandom::ClassID():
    element = new IEInitialRandom(reader);
    break;
  case IEClamp::ClassID():
    element = new IEClamp(reader);
    break;
  case IEPulse::ClassID():
    element = new IEPulse(reader);
    break;
  case IERealToInt::ClassID():
    element = new IERealToInt(reader);
    break;
  case IESubtract::ClassID():
    element = new IESubtract(reader);
    break;
  case IEGetCumulativeParticleCount::ClassID():
    element = new IEGetCumulativeParticleCount(reader);
    break;
  case IEGetActiveParticleCount::ClassID():
    element = new IEGetActiveParticleCount(reader);
    break;
  case IEGetEmitterTime::ClassID():
    element = new IEGetEmitterTime(reader);
    break;
  default:
    break;
  }

  if (element) {
    element->setPropertyName(propertyName);
  }
  return element;
}
IntElement* GetIntElement(const nlohmann::ordered_json& reader, const std::string_view propertyName) {
  IntElement* element = nullptr;
  if (!reader.contains(propertyName)) {
    return nullptr;
  }

  const auto type = GetClassID(reader[propertyName]);
  if (type == IENone::ClassName()) {
    element = new IENone(reader);
  } else if (type == IEConstant::ClassName()) {
    element = new IEConstant(reader);
  } else if (type == IEKeyframeEmitter::ClassName()) {
    element = new IEKeyframeEmitter(reader);
  } else if (type == IETimeScale::ClassName()) {
    element = new IETimeScale(reader);
  } else if (type == IEDeath::ClassName()) {
    element = new IEDeath(reader);
  } else if (type == IETimeChain::ClassName()) {
    element = new IETimeChain(reader);
  } else if (type == IEAdd::ClassName()) {
    element = new IEAdd(reader);
  } else if (type == IEMultiply::ClassName()) {
    element = new IEMultiply(reader);
  } else if (type == IEModulo::ClassName()) {
    element = new IEModulo(reader);
  } else if (type == IERandom::ClassName()) {
    element = new IERandom(reader);
  } else if (type == IEImpulse::ClassName()) {
    element = new IEImpulse(reader);
  } else if (type == IELifetimePercent::ClassName()) {
    element = new IELifetimePercent(reader);
  } else if (type == IESampleAndHold::ClassName()) {
    element = new IESampleAndHold(reader);
  } else if (type == IEInitialRandom::ClassName()) {
    element = new IEInitialRandom(reader);
  } else if (type == IEClamp::ClassName()) {
    element = new IEClamp(reader);
  } else if (type == IEPulse::ClassName()) {
    element = new IEPulse(reader);
  } else if (type == IERealToInt::ClassName()) {
    element = new IERealToInt(reader);
  } else if (type == IESubtract::ClassName()) {
    element = new IESubtract(reader);
  } else if (type == IEGetCumulativeParticleCount::ClassName()) {
    element = new IEGetCumulativeParticleCount(reader);
  } else if (type == IEGetActiveParticleCount::ClassName()) {
    element = new IEGetActiveParticleCount(reader);
  } else if (type == IEGetEmitterTime::ClassName()) {
    element = new IEGetEmitterTime(reader);
  }

  if (element) {
    element->setPropertyName(propertyName);
  }
  return element;
}

RealElement* GetRealElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  RealElement* element = nullptr;
  switch (GetClassID(reader)) {
  case RENone::ClassID():
    element = new RENone(reader);
    break;
  case REConstant::ClassID():
    element = new REConstant(reader);
    break;
  case REKeyframeEmitter::ClassIDNormal():
  case REKeyframeEmitter::ClassIDPercent():
    element = new REKeyframeEmitter(reader);
    break;
  case RETimeScale::ClassID():
    element = new RETimeScale(reader);
    break;
  case RESineWave::ClassID():
    element = new RESineWave(reader);
    break;
  case REAdd::ClassID():
    element = new REAdd(reader);
    break;
  case REMultiply::ClassID():
    element = new REMultiply(reader);
    break;
  case REDotProduct::ClassID():
    element = new REDotProduct(reader);
    break;
  case RERandom::ClassID():
    element = new RERandom(reader);
    break;
  case REInitialRandom::ClassID():
    element = new REInitialRandom(reader);
    break;
  case RETimeChain::ClassID():
    element = new RETimeChain(reader);
    break;
  case REClamp::ClassID():
    element = new REClamp(reader);
    break;
  case REPulse::ClassID():
    element = new REPulse(reader);
    break;
  case RELifetimePercent::ClassID():
    element = new RELifetimePercent(reader);
    break;
  case RELifetimeTween::ClassID():
    element = new RELifetimeTween(reader);
    break;
  case REParticleRotationOrLineWidth::ClassID():
    element = new REParticleRotationOrLineWidth(reader);
    break;
  case REParticleSizeOrLineLength::ClassID():
    element = new REParticleSizeOrLineLength(reader);
    break;
  case REParticleAccessParameter1::ClassID():
    element = new REParticleAccessParameter1(reader);
    break;
  case REParticleAccessParameter2::ClassID():
    element = new REParticleAccessParameter2(reader);
    break;
  case REParticleAccessParameter3::ClassID():
    element = new REParticleAccessParameter3(reader);
    break;
  case REParticleAccessParameter4::ClassID():
    element = new REParticleAccessParameter4(reader);
    break;
  case REParticleAccessParameter5::ClassID():
    element = new REParticleAccessParameter5(reader);
    break;
  case REParticleAccessParameter6::ClassID():
    element = new REParticleAccessParameter6(reader);
    break;
  case REParticleAccessParameter7::ClassID():
    element = new REParticleAccessParameter7(reader);
    break;
  case REParticleAccessParameter8::ClassID():
    element = new REParticleAccessParameter8(reader);
    break;
  case REVectorXToReal::ClassID():
    element = new REVectorXToReal(reader);
    break;
  case REVectorYToReal::ClassID():
    element = new REVectorYToReal(reader);
    break;
  case REVectorZToReal::ClassID():
    element = new REVectorZToReal(reader);
    break;
  case REVectorMagnitude::ClassID():
    element = new REVectorMagnitude(reader);
    break;
  case RECompareLessThan::ClassID():
    element = new RECompareLessThan(reader);
    break;
  case RECompareEqual::ClassID():
    element = new RECompareEqual(reader);
    break;
  case REConstantRange::ClassID():
    element = new REConstantRange(reader);
    break;
  case REExternalVar::ClassID():
    element = new REExternalVar(reader);
    break;
  case REIntTimesReal::ClassID():
    element = new REIntTimesReal(reader);
    break;
  case RESubtract::ClassID():
    element = new RESubtract(reader);
    break;
  case REGetComponentRed::ClassID():
    element = new REGetComponentRed(reader);
    break;
  case REGetComponentGreen::ClassID():
    element = new REGetComponentGreen(reader);
    break;
  case REGetComponentBlue::ClassID():
    element = new REGetComponentBlue(reader);
    break;
  case REGetComponentAlpha::ClassID():
    element = new REGetComponentAlpha(reader);
    break;
  default:
    break;
  }
  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

RealElement* GetRealElement(const nlohmann::ordered_json& reader, const std::string_view propertyName) {
  RealElement* element = nullptr;
  if (!reader.contains(propertyName)) {
    return nullptr;
  }
  const auto type = GetClassID(reader[propertyName]);
  if (type == RENone::ClassName()) {
    element = new RENone(reader);
  }
  if (type == REConstant::ClassName()) {
    element = new REConstant(reader);
  }
  if (type == REKeyframeEmitter::ClassName()) {
    element = new REKeyframeEmitter(reader);
  }
  if (type == RETimeScale::ClassName()) {
    element = new RETimeScale(reader);
  }
  if (type == RESineWave::ClassName()) {
    element = new RESineWave(reader);
  }
  if (type == REAdd::ClassName()) {
    element = new REAdd(reader);
  }
  if (type == REMultiply::ClassName()) {
    element = new REMultiply(reader);
  }
  if (type == REDotProduct::ClassName()) {
    element = new REDotProduct(reader);
  }
  if (type == RERandom::ClassName()) {
    element = new RERandom(reader);
  }
  if (type == REInitialRandom::ClassName()) {
    element = new REInitialRandom(reader);
  }
  if (type == RETimeChain::ClassName()) {
    element = new RETimeChain(reader);
  }
  if (type == REClamp::ClassName()) {
    element = new REClamp(reader);
  }
  if (type == REPulse::ClassName()) {
    element = new REPulse(reader);
  }
  if (type == RELifetimePercent::ClassName()) {
    element = new RELifetimePercent(reader);
  }
  if (type == RELifetimeTween::ClassName()) {
    element = new RELifetimeTween(reader);
  }
  if (type == REParticleRotationOrLineWidth::ClassName()) {
    element = new REParticleRotationOrLineWidth(reader);
  }
  if (type == REParticleAccessParameter1::ClassName()) {
    element = new REParticleAccessParameter1(reader);
  }
  if (type == REParticleAccessParameter2::ClassName()) {
    element = new REParticleAccessParameter2(reader);
  }
  if (type == REParticleAccessParameter3::ClassName()) {
    element = new REParticleAccessParameter3(reader);
  }
  if (type == REParticleAccessParameter4::ClassName()) {
    element = new REParticleAccessParameter4(reader);
  }
  if (type == REParticleAccessParameter5::ClassName()) {
    element = new REParticleAccessParameter5(reader);
  }
  if (type == REParticleAccessParameter6::ClassName()) {
    element = new REParticleAccessParameter6(reader);
  }
  if (type == REParticleAccessParameter7::ClassName()) {
    element = new REParticleAccessParameter7(reader);
  }
  if (type == REParticleAccessParameter8::ClassName()) {
    element = new REParticleAccessParameter8(reader);
  }
  if (type == REVectorXToReal::ClassName()) {
    element = new REVectorXToReal(reader);
  }
  if (type == REVectorYToReal::ClassName()) {
    element = new REVectorYToReal(reader);
  }
  if (type == REVectorZToReal::ClassName()) {
    element = new REVectorZToReal(reader);
  }
  if (type == REVectorMagnitude::ClassName()) {
    element = new REVectorMagnitude(reader);
  }
  if (type == RECompareLessThan::ClassName()) {
    element = new RECompareLessThan(reader);
  }
  if (type == RECompareEqual::ClassName()) {
    element = new RECompareEqual(reader);
  }
  if (type == REConstantRange::ClassName()) {
    element = new REConstantRange(reader);
  }
  if (type == RESubtract::ClassName()) {
    element = new RESubtract(reader);
  }
  if (type == REGetComponentRed::ClassName()) {
    element = new REGetComponentRed(reader);
  }
  if (type == REGetComponentGreen::ClassName()) {
    element = new REGetComponentGreen(reader);
  }
  if (type == REGetComponentBlue::ClassName()) {
    element = new REGetComponentBlue(reader);
  }
  if (type == REGetComponentAlpha::ClassName()) {
    element = new REGetComponentAlpha(reader);
  }
  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

VectorElement* GetVectorElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  VectorElement* element = nullptr;
  switch (GetClassID(reader)) {
  case VENone::ClassID():
    element = new VENone(reader);
    break;
  case VEConstant::ClassID():
    element = new VEConstant(reader);
    break;
  case VEKeyframeEmitter::ClassIDPercent():
  case VEKeyframeEmitter::ClassIDNormal():
    element = new VEKeyframeEmitter(reader);
    break;
  case VETimeChain::ClassID():
    element = new VETimeChain(reader);
    break;
  case VEAngleCone::ClassID():
    element = new VEAngleCone(reader);
    break;
  case VEAdd::ClassID():
    element = new VEAdd(reader);
    break;
  case VECircleCluster::ClassID():
    element = new VECircleCluster(reader);
    break;
  case VECircle::ClassID():
    element = new VECircle(reader);
    break;
  case VEMultiply::ClassID():
    element = new VEMultiply(reader);
    break;
  case VERealToVector::ClassID():
    element = new VERealToVector(reader);
    break;
  case VEPulse::ClassID():
    element = new VEPulse(reader);
    break;
  case VEParticleVelocity::ClassID():
    element = new VEParticleVelocity(reader);
    break;
  case VEParticleColor::ClassID():
    element = new VEParticleColor(reader);
    break;
  case VEParticleLocation::ClassID():
    element = new VEParticleLocation(reader);
    break;
  case VEParticleSystemOrientationFront::ClassID():
    element = new VEParticleSystemOrientationFront(reader);
    break;
  case VEParticleSystemOrientationUp::ClassID():
    element = new VEParticleSystemOrientationUp(reader);
    break;
  case VEParticleSystemOrientationRight::ClassID():
    element = new VEParticleSystemOrientationRight(reader);
    break;
  case VEParticleSystemTranslation::ClassID():
    element = new VEParticleSystemTranslation(reader);
    break;
  case VESubtract::ClassID():
    element = new VESubtract(reader);
    break;
  case VEColorToVector::ClassID():
    element = new VEColorToVector(reader);
    break;
  default:
    break;
  }
  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}
VectorElement* GetVectorElement(const nlohmann::ordered_json& reader, const std::string_view propertyName) {
  VectorElement* element = nullptr;
  if (!reader.contains(propertyName)) {
    return nullptr;
  }
  const auto type = GetClassID(reader[propertyName]);
  if (type == VENone::ClassName()) {
    element = new VENone(reader);
  } else if (type == VEConstant::ClassName()) {
    element = new VEConstant(reader);
  } else if (type == VEKeyframeEmitter::ClassName()) {
    element = new VEKeyframeEmitter(reader);
  } else if (type == VETimeChain::ClassName()) {
    element = new VETimeChain(reader);
  } else if (type == VEAngleCone::ClassName()) {
    element = new VEAngleCone(reader);
  } else if (type == VECircleCluster::ClassName()) {
    element = new VECircleCluster(reader);
  } else if (type == VECircle::ClassName()) {
    element = new VECircle(reader);
  } else if (type == VEMultiply::ClassName()) {
    element = new VEMultiply(reader);
  } else if (type == VERealToVector::ClassName()) {
    element = new VERealToVector(reader);
  } else if (type == VEPulse::ClassName()) {
    element = new VEPulse(reader);
  } else if (type == VEParticleVelocity::ClassName()) {
    element = new VEParticleVelocity(reader);
  } else if (type == VEParticleColor::ClassName()) {
    element = new VEParticleColor(reader);
  } else if (type == VEParticleLocation::ClassName()) {
    element = new VEParticleLocation(reader);
  } else if (type == VEParticleSystemOrientationFront::ClassName()) {
    element = new VEParticleSystemOrientationFront(reader);
  } else if (type == VEParticleSystemOrientationUp::ClassName()) {
    element = new VEParticleSystemOrientationUp(reader);
  } else if (type == VEParticleSystemOrientationRight::ClassName()) {
    element = new VEParticleSystemOrientationRight(reader);
  } else if (type == VEParticleSystemTranslation::ClassName()) {
    element = new VEParticleSystemTranslation(reader);
  } else if (type == VESubtract::ClassName()) {
    element = new VESubtract(reader);
  } else if (type == VEColorToVector::ClassName()) {
    element = new VEColorToVector(reader);
  }

  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

EmitterElement* GetEmitterElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  EmitterElement* element = nullptr;

  switch (GetClassID(reader)) {
  case EESimpleEmitter::ClassID():
    element = new EESimpleEmitter(reader);
    break;
  default:
    break;
  }

  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

EmitterElement* GetEmitterElement(const nlohmann::ordered_json& reader, const std::string_view propertyName) {
  EmitterElement* element = nullptr;

  if (!reader.contains(propertyName)) {
    return nullptr;
  }

  const auto type = GetClassID(reader[propertyName]);
  if (type == EESimpleEmitter::ClassName()) {
    element = new EESimpleEmitter(reader);
  }
  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

ModVectorElement* GetModVectorElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  ModVectorElement* element = nullptr;

  switch (GetClassID(reader)) {
  case MVENone::ClassID():
    element = new MVENone(reader);
    break;
  case MVEConstant::ClassID():
    element = new MVEConstant(reader);
    break;
  case MVEImplosion::ClassID():
    element = new MVEImplosion(reader);
    break;
  case MVEBounce::ClassID():
    element = new MVEBounce(reader);
    break;
  default:
    break;
  }

  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

ModVectorElement* GetModVectorElement(const nlohmann::ordered_json& reader, const std::string_view propertyName) {
  ModVectorElement* element = nullptr;

  if (!reader.contains(propertyName)) {
    return nullptr;
  }
  const auto type = GetClassID(reader[propertyName]);
  if (type == MVENone::ClassName()) {
    element = new MVENone(reader);
  } else if (type == MVEConstant::ClassName()) {
    element = new MVEConstant(reader);
  } else if (type == MVEImplosion::ClassName()) {
    element = new MVEImplosion(reader);
  } else if (type == MVEBounce::ClassName()) {
    element = new MVEBounce(reader);
  }

  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

UVElement* GetUVElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  UVElement* element = nullptr;

  switch (GetClassID(reader)) {
  case UVENone::ClassID():
    element = new UVENone(reader);
    break;
  case UVEConstant32Big::ClassID():
    element = new UVEConstant32Big(reader);
    break;
  case UVEAnimated32Big::ClassID():
    element = new UVEAnimated32Big(reader);
    break;
  default:
    break;
  }

  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

UVElement* GetUVElement(const nlohmann::ordered_json& reader, const std::string_view propertyName) {
  UVElement* element = nullptr;

  if (!reader.contains(propertyName)) {
    return nullptr;
  }

  const auto type = GetClassID(reader[propertyName]);

  if (type == UVENone::ClassName()) {
    element = new UVENone(reader);
  } else if (type == UVEConstant32Big::ClassName()) {
    element = new UVEConstant32Big(reader);
  } else if (type == UVEAnimated32Big::ClassName()) {
    element = new UVEAnimated32Big(reader);
  }

  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

void SetElement(athena::io::IStreamWriter& writer, const IElement* element, const FourCC& classId) {
  if (element == nullptr) {
    return;
  }
  SetClassID(writer, classId);
  element->PutTo(writer);
}

} // namespace axdl::primedep::particles::ParticleDataFactory