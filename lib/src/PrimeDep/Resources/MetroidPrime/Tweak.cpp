#include "PrimeDep/Resources/MetroidPrime/Tweak.hpp"

#include <athena/MemoryReader.hpp>

namespace axdl::primedep::MetroidPrime {

std::shared_ptr<IResource> ITweak::loadSpecialCooked(const char* ptr, std::size_t size, const std::string_view name) {
  if (name == "Player"sv) {
    return std::make_shared<TweakPlayer>(ptr, size);
  }
  if (name == "PlayerRes"sv) {
    return std::make_shared<TweakPlayerRes>(ptr, size);
  }
  if (name == "PlayerControls"sv || name == "PlayerControls2"sv) {
    return std::make_shared<TweakPlayerControls>(ptr, size);
  }
  if (name == "PlayerGun"sv) {
    return std::make_shared<TweakPlayerGun>(ptr, size);
  }
  if (name == "CameraBob"sv) {
    return std::make_shared<TweakCameraBob>(ptr, size);
  }
  if (name == "Ball"sv) {
    return std::make_shared<TweakBall>(ptr, size);
  }
  if (name == "GunRes"sv) {
    return std::make_shared<TweakGunRes>(ptr, size);
  }
  if (name == "Particle"sv) {
    return std::make_shared<TweakParticle>(ptr, size);
  }
  if (name == "SlideShow"sv) {
    return std::make_shared<TweakSlideShow>(ptr, size);
  }
  if (name == "Game"sv) {
    return std::make_shared<TweakGame>(ptr, size);
  }
  if (name == "Gui"sv) {
    return std::make_shared<TweakGui>(ptr, size);
  }
  if (name == "GuiColors"sv) {
    return std::make_shared<TweakGuiColors>(ptr, size);
  }
  if (name == "Targeting"sv) {
    return std::make_shared<TweakTargeting>(ptr, size);
  }
  if (name == "AutoMapper"sv) {
    return std::make_shared<TweakAutoMapper>(ptr, size);
  }

  return nullptr;
}

TweakPlayer::TweakPlayer(const char* ptr, const std::size_t size) {
  athena::io::MemoryReader in(ptr, size, true);

  for (auto& v : m_maxTranslationalAcceleration) {
    v = in.readFloatBig();
  }

  for (auto& v : m_maxRotationalAcceleration) {
    v = in.readFloatBig();
  }

  for (auto& v : m_translationFriction) {
    v = in.readFloatBig();
  }

  for (auto& v : m_rotationFriction) {
    v = in.readFloatBig();
  }

  for (auto& v : m_rotationMaxSpeed) {
    v = in.readFloatBig();
  }

  for (auto& v : m_translationMaxSpeed) {
    v = in.readFloatBig();
  }

  m_normalGravAccel = in.readFloatBig();
  m_fluidGravAccel = in.readFloatBig();
  m_verticalJumpAccel = in.readFloatBig();
  m_horizontalJumpAccel = in.readFloatBig();
  m_verticalDoubleJumpAccel = in.readFloatBig();
  m_horizontalDoubleJumpAccel = in.readFloatBig();
  m_waterJumpFactor = in.readFloatBig();
  m_waterBallJumpFactor = in.readFloatBig();
  m_lavaJumpFactor = in.readFloatBig();
  m_lavaBallJumpFactor = in.readFloatBig();
  m_phazonJumpFactor = in.readFloatBig();
  m_phazonBallJumpFactor = in.readFloatBig();
  m_allowedJumpTime = in.readFloatBig();
  m_allowedDoubleJumpTime = in.readFloatBig();
  m_minDoubleJumpWindow = in.readFloatBig();
  m_maxDoubleJumpWindow = in.readFloatBig();
  x104_ = in.readFloatBig();
  m_minJumpTime = in.readFloatBig();
  m_minDoubleJumpTime = in.readFloatBig();
  m_allowedLedgeTime = in.readFloatBig();
  m_doubleJumpImpulse = in.readFloatBig();
  m_backwardsForceMultiplier = in.readFloatBig();
  m_bombJumpRadius = in.readFloatBig();
  m_bombJumpHeight = in.readFloatBig();
  m_eyeOffset = in.readFloatBig();
  m_turnSpeedMultiplier = in.readFloatBig();
  m_freeLookTurnSpeedMultiplier = in.readFloatBig();

  m_horizontalFreeLookAngleVel = in.readFloatBig();
  m_verticalFreeLookAngleVel = in.readFloatBig();
  m_freeLookSpeed = in.readFloatBig();
  m_freeLookSnapSpeed = in.readFloatBig();
  x140_ = in.readFloatBig();
  m_freeLookCenteredThresholdAngle = in.readFloatBig();

  m_freeLookCenteredTime = in.readFloatBig();
  m_freeLookDampenFactor = in.readFloatBig();
  m_leftDiv = in.readFloatBig();
  m_rightDiv = in.readFloatBig();

  m_freelookTurnsPlayer = in.readBool();
  x228_25_ = in.readBool();
  x228_26_ = in.readBool();
  m_moveDuringFreeLook = in.readBool();
  m_holdButtonsForFreeLook = in.readBool();
  m_twoButtonsForFreeLook = in.readBool();
  x228_30_ = in.readBool();
  x228_31_ = in.readBool();
  x229_24_ = in.readBool();
  m_aimWhenOrbitingPoint = in.readBool();
  m_stayInFreeLookWhileFiring = in.readBool();
  x229_27_ = in.readBool();
  x229_28_ = in.readBool();
  m_orbitFixedOffset = in.readBool();
  m_gunButtonTogglesHolster = in.readBool();
  m_gunNotFiringHolstersGun = in.readBool();
  m_fallingDoubleJump = in.readBool();
  m_impulseDoubleJump = in.readBool();
  m_firingCancelsCameraPitch = in.readBool();
  m_assistedAimingIgnoreHorizontal = in.readBool();
  m_assistedAimingIgnoreVertical = in.readBool();

  x22c_ = in.readFloatBig();
  x230_ = in.readFloatBig();
  m_aimMaxDistance = in.readFloatBig();

  x238_ = in.readFloatBig();
  x23c_ = in.readFloatBig();
  x240_ = in.readFloatBig();
  x244_ = in.readFloatBig();
  x248_ = in.readFloatBig();
  m_aimThresholdDistance = in.readFloatBig();
  x250_ = in.readFloatBig();
  x254_ = in.readFloatBig();
  m_aimBoxWidth = in.readFloatBig();
  m_aimBoxHeight = in.readFloatBig();
  m_aimTargetTimer = in.readFloatBig();
  m_aimAssistHorizontalAngle = in.readFloatBig();
  m_aimAssistVerticalAngle = in.readFloatBig();

  for (int i = 0; i < 3; ++i) {
    m_orbitMinDistance[i] = in.readFloatBig();
    m_orbitNormalDistance[i] = in.readFloatBig();
    m_orbitMaxDistance[i] = in.readFloatBig();
  }

  x17c_ = in.readFloatBig();
  m_orbitModeTimer = in.readFloatBig();
  m_orbitCameraSpeed = in.readFloatBig();
  m_orbitUpperAngle = in.readFloatBig();
  m_orbitLowerAngle = in.readFloatBig();
  m_orbitHorizAngle = in.readFloatBig();
  x194_ = in.readFloatBig();
  x198_ = in.readFloatBig();
  m_orbitMaxTargetDistance = in.readFloatBig();
  m_orbitMaxLockDistance = in.readFloatBig();
  m_orbitDistanceThreshold = in.readFloatBig();

  for (int i = 0; i < 2; ++i) {
    m_orbitScreenBoxHalfExtentX[i] = in.readUint32Big();
    m_orbitScreenBoxHalfExtentY[i] = in.readUint32Big();
    m_orbitScreenBoxCenterX[i] = in.readUint32Big();
    m_orbitScreenBoxCenterY[i] = in.readUint32Big();
    m_orbitZoneIdealX[i] = in.readUint32Big();
    m_orbitZoneIdealY[i] = in.readUint32Big();
  }

  m_orbitNearX = in.readFloatBig();
  m_orbitNearZ = in.readFloatBig();
  x1e0_ = in.readFloatBig();
  x1e4_ = in.readFloatBig();
  m_orbitFixedOffsetZDiff = in.readFloatBig();
  m_orbitZRange = in.readFloatBig();
  x1f0_ = in.readFloatBig();
  x1f4_ = in.readFloatBig();
  x1f8_ = in.readFloatBig();
  m_orbitPreventionTime = in.readFloatBig();
  m_dashEnabled = in.readBool();
  m_dashOnButtonRelease = in.readBool();
  m_dashButtonHoldCancelTime = in.readFloatBig();
  m_dashStrafeInputThreshold = in.readFloatBig();
  m_sidewaysDoubleJumpImpulse = in.readFloatBig();
  m_sidewaysVerticalDoubleJumpAccel = in.readFloatBig();
  m_sidewaysHorizontalDoubleJumpAccel = in.readFloatBig();
  m_scanningRange = in.readFloatBig();
  m_scanRetention = in.readBool();
  m_scanFreezesGame = in.readBool();
  m_orbitWhileScanning = in.readBool();
  m_scanMaxTargetDistance = in.readFloatBig();
  m_scanMaxLockDistance = in.readFloatBig();
  m_orbitDistanceMax = in.readFloatBig();
  m_grappleSwingLength = in.readFloatBig();
  m_grappleSwingPeriod = in.readFloatBig();
  m_grapplePullSpeedMin = in.readFloatBig();
  m_grappleCameraSpeed = in.readFloatBig();
  m_maxGrappleLockedTurnAlignDistance = in.readFloatBig();
  m_grapplePullSpeedProportion = in.readFloatBig();
  m_grapplePullSpeedMax = in.readFloatBig();
  m_grappleLookCenterSpeed = in.readFloatBig();
  m_maxGrappleTurnSpeed = in.readFloatBig();
  m_grappleJumpForce = in.readFloatBig();
  m_grappleReleaseTime = in.readFloatBig();
  m_grappleJumpMode = in.readUint32Big();
  m_orbitReleaseBreaksGrapple = in.readBool();
  m_invertGrappleTurn = in.readBool();
  m_grappleBeamSpeed = in.readFloatBig();
  m_grappleBeamXWaveAmplitude = in.readFloatBig();
  m_grappleBeamZWaveAmplitude = in.readFloatBig();
  m_grappleBeamAnglePhaseDelta = in.readFloatBig();
  m_playerHeight = in.readFloatBig();
  m_playerXYHalfExtent = in.readFloatBig();
  m_stepUpHeight = in.readFloatBig();
  m_stepDownHeight = in.readFloatBig();
  m_playerBallHalfExtent = in.readFloatBig();
  m_firstPersonCameraSpeed = in.readFloatBig();
  x284_ = in.readFloatBig();
  m_jumpCameraPitchDownStart = in.readFloatBig();
  m_jumpCameraPitchDownFull = in.readFloatBig();
  m_jumpCameraPitchDownAngle = in.readFloatBig();
  m_fallCameraPitchDownStart = in.readFloatBig();
  m_fallCameraPitchDownFull = in.readFloatBig();
  m_fallCameraPitchDownAngle = in.readFloatBig();
  x2e8_ = in.readFloatBig();
  x2ec_ = in.readFloatBig();
  x2f0_ = in.readFloatBig();
  x2f4_ = in.readBool();
  m_frozenTimeout = in.readFloatBig();
  m_iceBreakJumpCount = in.readUint32Big();
  m_variaDamageReduction = in.readFloatBig();
  m_gravityDamageReduction = in.readFloatBig();
  m_phazonDamageReduction = in.readFloatBig();
}

bool TweakPlayer::writeUncooked(const std::string_view path) const {
  const auto p = rawPath(path);
  nlohmann::ordered_json out;

  out["MaxTranslationalAcceleration"] = m_maxTranslationalAcceleration;
  out["MaxRotationalAcceleration"] = m_maxRotationalAcceleration;
  out["TranslationFriction"] = m_translationFriction;
  out["RotationFriction"] = m_rotationFriction;
  out["RotationMaxSpeed"] = m_rotationMaxSpeed;
  out["TranslationMaxSpeed"] = m_translationMaxSpeed;
  out["NormalGravAccel"] = m_normalGravAccel;
  out["FluidGravAccel"] = m_fluidGravAccel;
  out["VerticalJumpAccel"] = m_verticalJumpAccel;
  out["HorizontalJumpAccel"] = m_horizontalJumpAccel;
  out["VerticalDoubleJumpAccel"] = m_verticalDoubleJumpAccel;
  out["HorizontalDoubleJumpAccel"] = m_horizontalDoubleJumpAccel;
  out["WaterJumpFactor"] = m_waterJumpFactor;
  out["WaterBallJumpFactor"] = m_waterBallJumpFactor;
  out["LavaJumpFactor"] = m_lavaJumpFactor;
  out["LavaBallJumpFactor"] = m_lavaBallJumpFactor;
  out["PhazonJumpFactor"] = m_phazonJumpFactor;
  out["PhazonBallJumpFactor"] = m_phazonBallJumpFactor;
  out["AllowedJumpTime"] = m_allowedJumpTime;
  out["AllowedDoubleJumpTime"] = m_allowedDoubleJumpTime;
  out["MinDoubleJumpWindow"] = m_minDoubleJumpWindow;
  out["MaxDoubleJumpWindow"] = m_maxDoubleJumpWindow;
  out["X104-NeedsName"] = x104_;
  out["MinJumpTime"] = m_minJumpTime;
  out["MinDoubleJumpTime"] = m_minDoubleJumpTime;
  out["AllowedLedgeTime"] = m_allowedLedgeTime;
  out["DoubleJumpImpulse"] = m_doubleJumpImpulse;
  out["BackwardsForceMultiplier"] = m_backwardsForceMultiplier;
  out["BombJumpRadius"] = m_bombJumpRadius;
  out["BombJumpHeight"] = m_bombJumpHeight;
  out["EyeOffset"] = m_eyeOffset;
  out["TurnSpeedMultiplier"] = m_turnSpeedMultiplier;
  out["FreeLookTurnSPeedMultiplier"] = m_freeLookTurnSpeedMultiplier;
  out["HorizontalFreeLookAngleVelocity"] = m_horizontalFreeLookAngleVel;
  out["VerticalFreeLookAngleVelocity"] = m_verticalFreeLookAngleVel;
  out["FreeLookSpeed"] = m_freeLookSpeed;
  out["FreeLookSnapSpeed"] = m_freeLookSnapSpeed;
  out["X140-NeedsName"] = x140_;
  out["FreeLookCenteredThresholdAngle"] = m_freeLookCenteredThresholdAngle;
  out["FreeLookCenteredTime"] = m_freeLookCenteredTime;
  out["FreeLookDampenFactor"] = m_freeLookDampenFactor;
  out["LeftStickDivisor"] = m_leftDiv;
  out["RightStickDivisor"] = m_rightDiv;
  out["OrbitMinDistance"] = m_orbitMinDistance;
  out["OrbitNormalDistance"] = m_orbitNormalDistance;
  out["OrbitMaxDistance"] = m_orbitMaxDistance;
  out["X174-NeedsName"] = x17c_;
  out["OrbitModeTimer"] = m_orbitModeTimer;
  out["OrbitCameraSpeed"] = m_orbitCameraSpeed;
  out["OrbitUpperAngle"] = m_orbitUpperAngle;
  out["OrbitLowerAngle"] = m_orbitLowerAngle;
  out["OrbitHorizontalAngle"] = m_orbitHorizAngle;
  out["X194-NeedsName"] = x194_;
  out["X198-NeedsName"] = x198_;
  out["OrbitMaxTargetDistance"] = m_orbitMaxTargetDistance;
  out["OrbitMaxLockDistance"] = m_orbitMaxLockDistance;
  out["OrbitDistanceThreshold"] = m_orbitDistanceThreshold;
  out["OrbitScreenBoxHalfExtentX"] = m_orbitScreenBoxHalfExtentX;
  out["OrbitScreenBoxHalfExtentY"] = m_orbitScreenBoxHalfExtentY;
  out["OrbitScreenBoxCenterX"] = m_orbitScreenBoxCenterX;
  out["OrbitScreenBoxCenterY"] = m_orbitScreenBoxCenterY;
  out["OrbitZoneIdealX"] = m_orbitZoneIdealX;
  out["OrbitZoneIdealY"] = m_orbitZoneIdealY;
  out["OrbitNearX"] = m_orbitNearX;
  out["OrbitNearZ"] = m_orbitNearZ;
  out["X1E0-NeedsName"] = x1e0_;
  out["X1E4-NeedsName"] = x1e4_;
  out["OrbitFixedOffsetZDiff"] = m_orbitFixedOffsetZDiff;
  out["OrbitZRange"] = m_orbitZRange;
  out["X1F0-NeedsName"] = x1f0_;
  out["X1F4-NeedsName"] = x1f4_;
  out["X1F8-NeedsName"] = x1f8_;
  out["OrbitPreventionTime"] = m_orbitPreventionTime;
  out["DashEnabled"] = m_dashEnabled;
  out["DashOnButtonRelease"] = m_dashOnButtonRelease;
  out["DashButtonHoldCancelTime"] = m_dashButtonHoldCancelTime;
  out["DashStrafeInputThreshold"] = m_dashStrafeInputThreshold;
  out["SidewaysDoubleJumpImpulse"] = m_sidewaysDoubleJumpImpulse;
  out["SidewaysVerticalDoubleJumpAccel"] = m_sidewaysVerticalDoubleJumpAccel;
  out["SidewaysHorizonalDoubleJumpAccel"] = m_sidewaysHorizontalDoubleJumpAccel;

  athena::io::FileWriter writer(p.generic_string());
  const auto json = out.dump(4) + "\n";
  writer.writeString(json, json.length());
  return !writer.hasError();
}

} // namespace axdl::primedep::MetroidPrime