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

#include <iostream>
#include <nlohmann/json.hpp>

namespace axdl::primedep::particles::ParticleDataFactory {
FourCC GetClassID(athena::io::IStreamReader& reader) { return FourCC(reader); }
std::string GetClassID(const nlohmann::ordered_json& data) { return data.value("Type", std::string()); }

FourCC PeekClassID(athena::io::IStreamReader& reader) {
  // Store old position so we can restore it after reading the 4cc
  const auto oldPos = reader.position();
  const FourCC ret{reader};
  // Restore the old position, we want to restore it rather than simply seek back 4 byte to prevent drifting back 4
  // bytes on failed reads
  reader.seek(oldPos, athena::SeekOrigin::Begin);
  return ret;
}

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
  SetClassID(writer, FOURCC('CNST'));
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

  const auto clsId = GetClassID(reader);
  switch (clsId) {
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
    std::cout << "[ColorElement] Unhandled element type " << clsId.toString() << std::endl;
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
  const auto property = reader[propertyName];
  const auto type = GetClassID(property);
  if (type == CENone::ClassName()) {
    element = new CENone(property);
  } else if (type == CEConstant::ClassName()) {
    element = new CEConstant(property);
  } else if (type == CEKeyframeEmitter::ClassName()) {
    element = new CEKeyframeEmitter(property);
  } else if (type == CETimeChain::ClassName()) {
    element = new CETimeChain(property);
  } else if (type == CEFadeEnd::ClassName()) {
    element = new CEFadeEnd(property);
  } else if (type == CEFade::ClassName()) {
    element = new CEFade(property);
  } else if (type == CEPulse::ClassName()) {
    element = new CEPulse(property);
  } else if (type == CEParticleColor::ClassName()) {
    element = new CEParticleColor(property);
  }
  if (element) {
    element->setPropertyName(propertyName);
  }
  return element;
}

IntElement* GetIntElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  IntElement* element = nullptr;

  const auto clsId = GetClassID(reader);
  switch (clsId) {
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
    std::cout << "[IntElement] Unhandled element type " << clsId.toString() << std::endl;
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
  const auto property = reader[propertyName];
  const auto type = GetClassID(reader[propertyName]);
  if (type == IENone::ClassName()) {
    element = new IENone(property);
  } else if (type == IEConstant::ClassName()) {
    element = new IEConstant(property);
  } else if (type == IEKeyframeEmitter::ClassName()) {
    element = new IEKeyframeEmitter(property);
  } else if (type == IETimeScale::ClassName()) {
    element = new IETimeScale(property);
  } else if (type == IEDeath::ClassName()) {
    element = new IEDeath(property);
  } else if (type == IETimeChain::ClassName()) {
    element = new IETimeChain(property);
  } else if (type == IEAdd::ClassName()) {
    element = new IEAdd(property);
  } else if (type == IEMultiply::ClassName()) {
    element = new IEMultiply(property);
  } else if (type == IEModulo::ClassName()) {
    element = new IEModulo(property);
  } else if (type == IERandom::ClassName()) {
    element = new IERandom(property);
  } else if (type == IEImpulse::ClassName()) {
    element = new IEImpulse(property);
  } else if (type == IELifetimePercent::ClassName()) {
    element = new IELifetimePercent(property);
  } else if (type == IESampleAndHold::ClassName()) {
    element = new IESampleAndHold(property);
  } else if (type == IEInitialRandom::ClassName()) {
    element = new IEInitialRandom(property);
  } else if (type == IEClamp::ClassName()) {
    element = new IEClamp(property);
  } else if (type == IEPulse::ClassName()) {
    element = new IEPulse(property);
  } else if (type == IERealToInt::ClassName()) {
    element = new IERealToInt(property);
  } else if (type == IESubtract::ClassName()) {
    element = new IESubtract(property);
  } else if (type == IEGetCumulativeParticleCount::ClassName()) {
    element = new IEGetCumulativeParticleCount(property);
  } else if (type == IEGetActiveParticleCount::ClassName()) {
    element = new IEGetActiveParticleCount(property);
  } else if (type == IEGetEmitterTime::ClassName()) {
    element = new IEGetEmitterTime(property);
  }

  if (element) {
    element->setPropertyName(propertyName);
  }
  return element;
}

RealElement* GetRealElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  RealElement* element = nullptr;
  const auto clsId = GetClassID(reader);
  switch (clsId) {
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
  case REInitialSwitch::ClassID():
    element = new REInitialSwitch(reader);
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
    std::cout << "[RealElement] Unhandled element type " << clsId.toString() << std::endl;
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

  const auto property = reader[propertyName];
  const auto type = GetClassID(property);
  if (type == RENone::ClassName()) {
    element = new RENone(property);
  }
  if (type == REConstant::ClassName()) {
    element = new REConstant(property);
  }
  if (type == REKeyframeEmitter::ClassName()) {
    element = new REKeyframeEmitter(property);
  }
  if (type == RETimeScale::ClassName()) {
    element = new RETimeScale(property);
  }
  if (type == RESineWave::ClassName()) {
    element = new RESineWave(property);
  }
  if (type == REAdd::ClassName()) {
    element = new REAdd(property);
  }
  if (type == REMultiply::ClassName()) {
    element = new REMultiply(property);
  }
  if (type == REDotProduct::ClassName()) {
    element = new REDotProduct(property);
  }
  if (type == RERandom::ClassName()) {
    element = new RERandom(property);
  }
  if (type == REInitialRandom::ClassName()) {
    element = new REInitialRandom(property);
  }
  if (type == RETimeChain::ClassName()) {
    element = new RETimeChain(property);
  }
  if (type == REClamp::ClassName()) {
    element = new REClamp(property);
  }
  if (type == REPulse::ClassName()) {
    element = new REPulse(property);
  }
  if (type == RELifetimePercent::ClassName()) {
    element = new RELifetimePercent(property);
  }
  if (type == RELifetimeTween::ClassName()) {
    element = new RELifetimeTween(property);
  }
  if (type == REParticleRotationOrLineWidth::ClassName()) {
    element = new REParticleRotationOrLineWidth(property);
  }
  if (type == REParticleAccessParameter1::ClassName()) {
    element = new REParticleAccessParameter1(property);
  }
  if (type == REParticleAccessParameter2::ClassName()) {
    element = new REParticleAccessParameter2(property);
  }
  if (type == REParticleAccessParameter3::ClassName()) {
    element = new REParticleAccessParameter3(property);
  }
  if (type == REParticleAccessParameter4::ClassName()) {
    element = new REParticleAccessParameter4(property);
  }
  if (type == REParticleAccessParameter5::ClassName()) {
    element = new REParticleAccessParameter5(property);
  }
  if (type == REParticleAccessParameter6::ClassName()) {
    element = new REParticleAccessParameter6(property);
  }
  if (type == REParticleAccessParameter7::ClassName()) {
    element = new REParticleAccessParameter7(property);
  }
  if (type == REParticleAccessParameter8::ClassName()) {
    element = new REParticleAccessParameter8(property);
  }
  if (type == REVectorXToReal::ClassName()) {
    element = new REVectorXToReal(property);
  }
  if (type == REVectorYToReal::ClassName()) {
    element = new REVectorYToReal(property);
  }
  if (type == REVectorZToReal::ClassName()) {
    element = new REVectorZToReal(property);
  }
  if (type == REVectorMagnitude::ClassName()) {
    element = new REVectorMagnitude(property);
  }
  if (type == RECompareLessThan::ClassName()) {
    element = new RECompareLessThan(property);
  }
  if (type == RECompareEqual::ClassName()) {
    element = new RECompareEqual(property);
  }
  if (type == REConstantRange::ClassName()) {
    element = new REConstantRange(property);
  }
  if (type == RESubtract::ClassName()) {
    element = new RESubtract(property);
  }
  if (type == REGetComponentRed::ClassName()) {
    element = new REGetComponentRed(property);
  }
  if (type == REGetComponentGreen::ClassName()) {
    element = new REGetComponentGreen(property);
  }
  if (type == REGetComponentBlue::ClassName()) {
    element = new REGetComponentBlue(property);
  }
  if (type == REGetComponentAlpha::ClassName()) {
    element = new REGetComponentAlpha(property);
  }
  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

VectorElement* GetVectorElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  VectorElement* element = nullptr;
  const auto clsId = GetClassID(reader);
  switch (clsId) {
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
  case VEAngleCone::ClassID():
    element = new VEAngleCone(reader);
    break;
  case VECone::ClassID():
    element = new VECone(reader);
    break;
  case VECircle::ClassID():
    element = new VECircle(reader);
    break;
  case VECircleCluster::ClassID():
    element = new VECircleCluster(reader);
    break;
  case VEAdd::ClassID():
    element = new VEAdd(reader);
    break;
  case VEMultiply::ClassID():
    element = new VEMultiply(reader);
    break;
  case VETimeChain::ClassID():
    element = new VETimeChain(reader);
    break;
  case VEPulse::ClassID():
    element = new VEPulse(reader);
    break;
  case VERealToVector::ClassID():
    element = new VERealToVector(reader);
    break;
  case VEParticleLocation::ClassID():
    element = new VEParticleLocation(reader);
    break;
  case VEParticleColor::ClassID():
    element = new VEParticleColor(reader);
    break;
  case VEParticleVelocity::ClassID():
    element = new VEParticleVelocity(reader);
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
    std::cout << "[VectorElement] Unhandled element type " << clsId.toString() << std::endl;
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
  const auto property = reader[propertyName];
  const auto type = GetClassID(property);
  if (type == VENone::ClassName()) {
    element = new VENone(property);
  } else if (type == VEConstant::ClassName()) {
    element = new VEConstant(property);
  } else if (type == VEKeyframeEmitter::ClassName()) {
    element = new VEKeyframeEmitter(property);
  } else if (type == VECone::ClassName()) {
    element = new VECone(property);
  } else if (type == VEAngleCone::ClassName()) {
    element = new VEAngleCone(property);
  } else if (type == VECircleCluster::ClassName()) {
    element = new VECircleCluster(property);
  } else if (type == VECircle::ClassName()) {
    element = new VECircle(property);
  } else if (type == VEAdd::ClassName()) {
    element = new VEAdd(property);
  } else if (type == VEMultiply::ClassName()) {
    element = new VEMultiply(property);
  } else if (type == VETimeChain::ClassName()) {
    element = new VETimeChain(property);
  } else if (type == VEPulse::ClassName()) {
    element = new VEPulse(property);
  } else if (type == VERealToVector::ClassName()) {
    element = new VERealToVector(property);
  } else if (type == VEParticleColor::ClassName()) {
    element = new VEParticleColor(property);
  } else if (type == VEParticleLocation::ClassName()) {
    element = new VEParticleLocation(property);
  } else if (type == VEParticleVelocity::ClassName()) {
    element = new VEParticleVelocity(property);
  } else if (type == VEParticleSystemOrientationFront::ClassName()) {
    element = new VEParticleSystemOrientationFront(property);
  } else if (type == VEParticleSystemOrientationUp::ClassName()) {
    element = new VEParticleSystemOrientationUp(property);
  } else if (type == VEParticleSystemOrientationRight::ClassName()) {
    element = new VEParticleSystemOrientationRight(property);
  } else if (type == VEParticleSystemTranslation::ClassName()) {
    element = new VEParticleSystemTranslation(property);
  } else if (type == VESubtract::ClassName()) {
    element = new VESubtract(property);
  } else if (type == VEColorToVector::ClassName()) {
    element = new VEColorToVector(property);
  }

  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

EmitterElement* GetEmitterElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  EmitterElement* element = nullptr;

  const auto clsId = GetClassID(reader);
  switch (clsId) {
  case EENone::ClassID():
    element = new EENone(reader);
    break;
  case EESimpleEmitter::ClassIDNew():
  case EESimpleEmitter::ClassIDOld():
    element = new EESimpleEmitter(reader);
    break;
  case VESphere::ClassID():
    element = new VESphere(reader);
    break;
  case VEAngleSphere::ClassID():
    element = new VEAngleSphere(reader);
    break;
  default:
    std::cout << "[EmitterElement] Unhandled element type " << clsId.toString() << std::endl;
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

  const auto property = !propertyName.empty() ? reader[propertyName] : reader;
  const auto type = GetClassID(property);
  if (type == EESimpleEmitter::ClassName()) {
    element = new EESimpleEmitter(property);
  } else if (type == VESphere::ClassName()) {
    element = new VESphere(property);
  } else if (type == VEAngleSphere::ClassName()) {
    element = new VEAngleSphere(property);
  }
  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

ModVectorElement* GetModVectorElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  ModVectorElement* element = nullptr;

  const auto clsId = GetClassID(reader);
  switch (clsId) {
  case MVENone::ClassID():
    element = new MVENone(reader);
    break;
  case MVEConstant::ClassID():
    element = new MVEConstant(reader);
    break;
  case MVEGravity::ClassID():
    element = new MVEGravity(reader);
    break;
  case MVEWind::ClassID():
    element = new MVEWind(reader);
    break;
  case MVEExplode::ClassID():
    element = new MVEExplode(reader);
    break;
  case MVETimeChain::ClassID():
    element = new MVETimeChain(reader);
    break;
  case MVEPulse::ClassID():
    element = new MVEPulse(reader);
    break;
  case MVEImplosion::ClassID():
    element = new MVEImplosion(reader);
    break;
  case MVELinearImplosion::ClassID():
    element = new MVELinearImplosion(reader);
    break;
  case MVEExponentialImplosion::ClassID():
    element = new MVEExponentialImplosion(reader);
    break;
  case MVESwirl::ClassID():
    element = new MVESwirl(reader);
    break;
  case MVEBounce::ClassID():
    element = new MVEBounce(reader);
    break;
  case MVESetPosition::ClassID():
    element = new MVESetPosition(reader);
    break;
  default:
    std::cout << "[ModVectorElement] Unhandled element type " << clsId.toString() << std::endl;
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
  const auto property = !propertyName.empty() ? reader[propertyName] : reader;
  const auto type = GetClassID(property);
  if (type == MVENone::ClassName()) {
    element = new MVENone(property);
  } else if (type == MVEConstant::ClassName()) {
    element = new MVEConstant(property);
  } else if (type == MVEGravity::ClassName()) {
    element = new MVEGravity(property);
  } else if (type == MVEWind::ClassName()) {
    element = new MVEWind(property);
  } else if (type == MVEExplode::ClassName()) {
    element = new MVEExplode(property);
  } else if (type == MVETimeChain::ClassName()) {
    element = new MVETimeChain(property);
  } else if (type == MVEPulse::ClassName()) {
    element = new MVEPulse(property);
  } else if (type == MVEImplosion::ClassName()) {
    element = new MVEImplosion(property);
  } else if (type == MVELinearImplosion::ClassName()) {
    element = new MVELinearImplosion(property);
  } else if (type == MVEExponentialImplosion::ClassName()) {
    element = new MVEExponentialImplosion(property);
  } else if (type == MVESwirl::ClassName()) {
    element = new MVESwirl(property);
  } else if (type == MVEBounce::ClassName()) {
    element = new MVEBounce(property);
  } else if (type == MVESetPosition::ClassName()) {
    element = new MVESetPosition(property);
  }

  if (element) {
    element->setPropertyName(propertyName);
  }

  return element;
}

UVElement* GetUVElement(athena::io::IStreamReader& reader, const std::string_view propertyName) {
  UVElement* element = nullptr;

  const auto clsId = GetClassID(reader);
  switch (clsId) {
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
    std::cout << "[UVElement] Unhandled element type " << clsId.toString() << std::endl;
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

  const auto property = reader[propertyName];
  const auto type = GetClassID(property);

  if (type == UVENone::ClassName()) {
    element = new UVENone(property);
  } else if (type == UVEConstant32Big::ClassName()) {
    element = new UVEConstant32Big(property);
  } else if (type == UVEAnimated32Big::ClassName()) {
    element = new UVEAnimated32Big(property);
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