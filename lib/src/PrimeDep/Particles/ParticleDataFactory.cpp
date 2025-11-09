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

bool GetBool(athena::io::IStreamReader& reader) {
  if (GetClassID(reader) != FOURCC('CNST')) {
    return false;
  }
  return reader.readBool();
}
bool GetBool(const nlohmann::ordered_json& reader, const std::string_view type) {
  if (type.empty()) {
    return false;
  }
  return reader.value(type, false);
}
void SetBool(athena::io::IStreamWriter& writer, const bool value) {
  SetClassID(writer, FOURCC('CNST'));
  writer.writeBool(value);
}
void SetBool(nlohmann::ordered_json& writer, const std::string_view type, const bool value) {
  if (type.empty()) {
    return;
  }
  writer[type] = value;
}

int GetInt(athena::io::IStreamReader& reader) { return reader.readInt32Big(); }
int GetInt(const nlohmann::ordered_json& reader, const std::string_view type) {
  if (type.empty()) {
    return 0;
  }
  return reader.value(type, 0);
}
void SetInt(athena::io::IStreamWriter& writer, const int value) { writer.writeInt32Big(value); }
void SetInt(nlohmann::ordered_json& writer, const std::string_view type, const int value) {
  if (type.empty()) {
    return;
  }

  writer[type] = value;
}

float GetReal(athena::io::IStreamReader& reader) { return reader.readFloatBig(); }
float GetReal(const nlohmann::ordered_json& reader, const std::string_view type) { return reader.value(type, 0); }
void SetReal(athena::io::IStreamWriter& writer, const float value) { writer.writeFloatBig(value); }
void SetReal(nlohmann::ordered_json& writer, const std::string_view type, const float value) {
  if (type.empty()) {
    return;
  }

  writer[type] = value;
}

std::optional<AssetId32Big> GetAssetID32Big(athena::io::IStreamReader& reader, const FourCC& type) {
  if (GetClassID(reader) == FOURCC('NONE')) {
    return {kInvalidAssetId32Big};
  }

  return {AssetId32Big(reader, type)};
}

ColorElement* GetColorElement(athena::io::IStreamReader& reader) {
  switch (GetClassID(reader)) {
  case SBIG('CNST'):
    return new CEConstant(reader);
  case SBIG('KEYE'):
  case SBIG('KEYP'):
    return new CEKeyframeEmitter(reader);
  case SBIG('CHAN'):
    return new CETimeChain(reader);
  case SBIG('CFDE'):
    return new CEFadeEnd(reader);
  case SBIG('FADE'):
    return new CEFade(reader);
  case SBIG('PULS'):
    return new CEPulse(reader);
  case SBIG('PCOL'):
    return new CEParticleColor(reader);
  case SBIG('NONE'):
    return new CENone();
  default:
    return nullptr;
  }
}

ColorElement* GetColorElement(const nlohmann::ordered_json& reader, const std::string_view type) { return nullptr; }
IntElement* GetIntElement(athena::io::IStreamReader& reader) {
  switch (GetClassID(reader)) {
  case SBIG('CNST'):
    return new IEConstant(reader);
  case SBIG('KEYE'):
  case SBIG('KEYP'):
    return new IEKeyframeEmitter(reader);
  case SBIG('TSCL'):
    return new IETimeScale(reader);
  case SBIG('DETH'):
    return new IEDeath(reader);
  case SBIG('CHAN'):
    return new IETimeChain(reader);
  case SBIG('ADD_'):
    return new IEAdd(reader);
  case SBIG('MULT'):
    return new IEMultiply(reader);
  case SBIG('MODU'):
    return new IEModulo(reader);
  case SBIG('RAND'):
    return new IERandom(reader);
  case SBIG('IMPL'):
    return new IEImpulse(reader);
  case SBIG('ILPT'):
    return new IELifetimePercent(reader);
  case SBIG('SPAH'):
    return new IESampleAndHold(reader);
  case SBIG('IRND'):
    return new IEInitialRandom(reader);
  case SBIG('CLMP'):
    return new IEClamp(reader);
  case SBIG('PULS'):
    return new IEPulse(reader);
  case SBIG('NONE'):
    return new IENone();
  case SBIG('RTOI'):
    return new IERealToInt(reader);
  case SBIG('SUB_'):
    return new IESubtract(reader);
  case SBIG('GTCP'):
    return new IEGetCumulativeParticleCount();
  case SBIG('GAPC'):
    return new IEGetActiveParticleCount();
  case SBIG('GEMT'):
    return new IEGetEmitterTime();
  default:
    return nullptr;
  }
}
IntElement* GetIntElement(const nlohmann::ordered_json& reader, const std::string_view type) { return nullptr; }

RealElement* GetRealElement(athena::io::IStreamReader& reader) {
  switch (GetClassID(reader)) {
  case SBIG('CNST'):
    return new REConstant(reader);
  case SBIG('NONE'):
    return new RENone();
  case SBIG('KEYE'):
  case SBIG('KEYP'):
    return new REKeyframeEmitter(reader);
  case SBIG('SCAL'):
    return new RETimeScale(reader);
  case SBIG('SINE'):
    return new RESineWave(reader);
  case SBIG('ADD_'):
    return new REAdd(reader);
  case SBIG('MULT'):
    return new REMultiply(reader);
  case SBIG('DOTP'):
    return new REDotProduct(reader);
  case SBIG('RAND'):
    return new RERandom(reader);
  case SBIG('IRND'):
    return new REInitialRandom(reader);
  case SBIG('CHAN'):
    return new RETimeChain(reader);
  case SBIG('CLMP'):
    return new REClamp(reader);
  case SBIG('PULS'):
    return new REPulse(reader);
  case SBIG('RLPT'):
    return new RELifetimePercent(reader);
  case SBIG('LFTW'):
    return new RELifetimeTween(reader);
  case SBIG('PRLW'):
    return new REParticleRotationOrLineWidth();
  case SBIG('PSLL'):
    return new REParticleSizeOrLineLength();
  case SBIG('PAP1'):
    return new REParticleAccessParameter1();
  case SBIG('PAP2'):
    return new REParticleAccessParameter2();
  case SBIG('PAP3'):
    return new REParticleAccessParameter3();
  case SBIG('PAP4'):
    return new REParticleAccessParameter4();
  case SBIG('PAP5'):
    return new REParticleAccessParameter5();
  case SBIG('PAP6'):
    return new REParticleAccessParameter6();
  case SBIG('PAP7'):
    return new REParticleAccessParameter7();
  case SBIG('PAP8'):
    return new REParticleAccessParameter8();
  case SBIG('VXTR'):
    return new REVectorXToReal(reader);
  case SBIG('VYTR'):
    return new REVectorYToReal(reader);
  case SBIG('VZTR'):
    return new REVectorZToReal(reader);
  case SBIG('VMAG'):
    return new REVectorMagnitude(reader);
  case SBIG('CLTN'):
    return new RECompareLessThan(reader);
  case SBIG('CEQL'):
    return new RECompareEqual(reader);
  case SBIG('CRNG'):
    return new REConstantRange(reader);
  case SBIG('CEXT'):
    return new REExternalVar(reader);
  case SBIG('SUB_'):
    return new RESubtract(reader);
  case SBIG('GTCR'):
    return new REGetComponentRed(reader);
  case SBIG('GTCG'):
    return new REGetComponentGreen(reader);
  case SBIG('GTCB'):
    return new REGetComponentBlue(reader);
  case SBIG('GTCA'):
    return new REGetComponentAlpha(reader);
  default:
    return nullptr;
  }
}

RealElement* GetRealElement(const nlohmann::ordered_json& reader, const std::string_view type) { return nullptr; }

VectorElement* GetVectorElement(athena::io::IStreamReader& reader) {
  switch (GetClassID(reader)) {
  case SBIG('NONE'):
    return new VENone();
  case SBIG('CNST'):
    return new VEConstant(reader);
  case SBIG('KEYE'):
  case SBIG('KEYP'):
    return new VEKeyframeEmitter(reader);
  case SBIG('CHAN'):
    return new VETimeChain(reader);
  case SBIG('BNCE'):
    return new VEBounce(reader);
  default:
    return nullptr;
  }
}
VectorElement* GetVectorElement(const nlohmann::ordered_json& reader, const std::string_view type) { return nullptr; }

EmitterElement* GetEmitterElement(athena::io::IStreamReader& reader) {
  switch (GetClassID(reader)) {
  case SBIG('SETR'): {
    return new EESimpleEmitter(reader);
  }
  default:
    return nullptr;
  }
}

EmitterElement* GetEmitterElement(const nlohmann::ordered_json& reader, const std::string_view type) { return nullptr; }

ModVectorElement* GetModVectorElement(athena::io::IStreamReader& reader) {
  switch (GetClassID(reader)) {
  case SBIG('CNST'):
    return new MVEConstant(reader);
  case SBIG('IMPL'):
    return new MVEImplosion(reader);
  case SBIG('NONE'):
  default:
    return nullptr;
  }
}

ModVectorElement* GetModVectorElement(const nlohmann::ordered_json& reader, const std::string_view type) {
  return nullptr;
}

UVElement* GetUVElement(athena::io::IStreamReader& reader) { return nullptr; }
UVElement* GetUVElement(const nlohmann::ordered_json& reader, const std::string_view type) { return nullptr; }
} // namespace axdl::primedep::particles::ParticleDataFactory