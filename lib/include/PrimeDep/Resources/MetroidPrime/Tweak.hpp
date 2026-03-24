#pragma once
#include "PrimeDep/IResource.hpp"

namespace axdl::primedep::MetroidPrime {
class TweakPlayer;
class TweakPlayerControls;
class TweakCameraBob;
class TweakGunRes;
class TweakParticle;
class TweakSlideShow;
class TweakAutoMapper;
class TweakBall;
class TweakGame;
class TweakGui;
class TweakGuiColors;
class ITweak : public TypedResource('CTWK', ".tweak", ".ctwk", "Game Engine Tweaks") {
public:
  static std::shared_ptr<IResource> loadSpecialCooked(const char* ptr, std::size_t size, const std::string_view name);
  static bool canIngest(const nlohmann::ordered_json& metadata) { return false; }

  static std::shared_ptr<IResource> ingest(const nlohmann::ordered_json& metadata, std::string_view path) {
    return nullptr;
  }
};

class TweakPlayer final : public ITweak {
public:
  TweakPlayer(const char* ptr, std::size_t size);

  bool writeCooked(std::string_view path) const override { return false; }
  bool writeUncooked(std::string_view path) const override;

private:
  std::array<float, 8> m_maxTranslationalAcceleration{};
  std::array<float, 8> m_maxRotationalAcceleration{};
  std::array<float, 8> m_translationFriction{};
  std::array<float, 8> m_rotationFriction{};
  std::array<float, 8> m_rotationMaxSpeed{};
  std::array<float, 8> m_translationMaxSpeed{};
  float m_normalGravAccel{};
  float m_fluidGravAccel{};
  float m_verticalJumpAccel;
  float m_horizontalJumpAccel{};
  float m_verticalDoubleJumpAccel{};
  float m_horizontalDoubleJumpAccel{};
  float m_waterJumpFactor{};
  float m_waterBallJumpFactor{};
  float m_lavaJumpFactor{};
  float m_lavaBallJumpFactor{};
  float m_phazonJumpFactor{};
  float m_phazonBallJumpFactor{};
  float m_allowedJumpTime{};
  float m_allowedDoubleJumpTime{};
  float m_minDoubleJumpWindow{};
  float m_maxDoubleJumpWindow{};
  float x104_{};
  float m_minJumpTime{};
  float m_minDoubleJumpTime{};
  float m_allowedLedgeTime{};
  float m_doubleJumpImpulse{};
  float m_backwardsForceMultiplier{};
  float m_bombJumpRadius{};
  float m_bombJumpHeight{};
  float m_eyeOffset{};
  float m_turnSpeedMultiplier{};
  float m_freeLookTurnSpeedMultiplier{};
  float m_horizontalFreeLookAngleVel{};
  float m_verticalFreeLookAngleVel{};
  float m_freeLookSpeed{};
  float m_freeLookSnapSpeed{};
  float x140_{};
  float m_freeLookCenteredThresholdAngle{};
  float m_freeLookCenteredTime{};
  float m_freeLookDampenFactor{};
  float m_leftDiv{};
  float m_rightDiv{};
  std::array<float, 3> m_orbitMinDistance{};
  std::array<float, 3> m_orbitNormalDistance{};
  std::array<float, 3> m_orbitMaxDistance{};
  float x17c_{};
  float m_orbitModeTimer{};
  float m_orbitCameraSpeed{};
  float m_orbitUpperAngle{};
  float m_orbitLowerAngle{};
  float m_orbitHorizAngle{};
  float x194_{};
  float x198_{};
  float m_orbitMaxTargetDistance{};
  float m_orbitMaxLockDistance{};
  float m_orbitDistanceThreshold{};
  std::array<uint32_t, 2> m_orbitScreenBoxHalfExtentX{};
  std::array<uint32_t, 2> m_orbitScreenBoxHalfExtentY{};
  std::array<uint32_t, 2> m_orbitScreenBoxCenterX{};
  std::array<uint32_t, 2> m_orbitScreenBoxCenterY{};
  std::array<uint32_t, 2> m_orbitZoneIdealX{};
  std::array<uint32_t, 2> m_orbitZoneIdealY{};
  float m_orbitNearX{};
  float m_orbitNearZ{};
  float x1e0_{};
  float x1e4_{};
  float m_orbitFixedOffsetZDiff{};
  float m_orbitZRange{};
  float x1f0_{};
  float x1f4_{};
  float x1f8_{};
  float m_orbitPreventionTime{};
  bool m_dashEnabled : 1 {};
  bool m_dashOnButtonRelease : 1 {};
  float m_dashButtonHoldCancelTime{};
  float m_dashStrafeInputThreshold{};
  float m_sidewaysDoubleJumpImpulse{};
  float m_sidewaysVerticalDoubleJumpAccel{};
  float m_sidewaysHorizontalDoubleJumpAccel{};
  float m_scanningRange{};
  bool m_scanRetention : 1 {};
  bool m_scanFreezesGame : 1 {};
  bool m_orbitWhileScanning : 1 {};
  float m_scanMaxTargetDistance{};
  float m_scanMaxLockDistance{};
  bool m_freelookTurnsPlayer : 1 {};
  bool x228_25_ : 1 {};
  bool x228_26_ : 1 {};
  bool m_moveDuringFreeLook : 1 {};
  bool m_holdButtonsForFreeLook : 1 {};
  bool m_twoButtonsForFreeLook : 1 {};
  bool x228_30_ : 1 {};
  bool x228_31_ : 1 {};
  bool x229_24_ : 1 {};
  bool m_aimWhenOrbitingPoint : 1 {};
  bool m_stayInFreeLookWhileFiring : 1 {};
  bool x229_27_ : 1 {};
  bool x229_28_ : 1 {};
  bool m_orbitFixedOffset : 1 {};
  bool m_gunButtonTogglesHolster : 1 {};
  bool m_gunNotFiringHolstersGun : 1 {};
  bool m_fallingDoubleJump : 1 {};
  bool m_impulseDoubleJump : 1 {};
  bool m_firingCancelsCameraPitch : 1 {};
  bool m_assistedAimingIgnoreHorizontal : 1 {};
  bool m_assistedAimingIgnoreVertical : 1 {};
  float x22c_{};
  float x230_{};
  float m_aimMaxDistance{};
  float x238_{};
  float x23c_{};
  float x240_{};
  float x244_{};
  float x248_{};
  float m_aimThresholdDistance{};
  float x250_{};
  float x254_{};
  float m_aimBoxWidth{};
  float m_aimBoxHeight{};
  float m_aimTargetTimer{};
  float m_aimAssistHorizontalAngle{};
  float m_aimAssistVerticalAngle{};
  float m_playerHeight{};
  float m_playerXYHalfExtent{};
  float m_stepUpHeight{};
  float m_stepDownHeight{};
  float m_playerBallHalfExtent{};
  float m_firstPersonCameraSpeed{};
  float x284_{};
  float m_jumpCameraPitchDownStart{};
  float m_jumpCameraPitchDownFull{};
  float m_jumpCameraPitchDownAngle{};
  float m_fallCameraPitchDownStart{};
  float m_fallCameraPitchDownFull{};
  float m_fallCameraPitchDownAngle{};
  float m_orbitDistanceMax{};
  float m_grappleSwingLength{};
  float m_grappleSwingPeriod{};
  float m_grapplePullSpeedMin{};
  float m_grappleCameraSpeed{};
  float m_maxGrappleLockedTurnAlignDistance{};
  float m_grapplePullSpeedProportion{};
  float m_grapplePullSpeedMax{};
  float m_grappleLookCenterSpeed{};
  float m_maxGrappleTurnSpeed{};
  float m_grappleJumpForce{};
  float m_grappleReleaseTime{};
  uint32_t m_grappleJumpMode{};
  bool m_orbitReleaseBreaksGrapple{};
  bool m_invertGrappleTurn{};
  float m_grappleBeamSpeed{};
  float m_grappleBeamXWaveAmplitude{};
  float m_grappleBeamZWaveAmplitude{};
  float m_grappleBeamAnglePhaseDelta{};
  float x2e8_{};
  float x2ec_{};
  float x2f0_{};
  bool x2f4_{};
  float m_frozenTimeout{};
  int m_iceBreakJumpCount{};
  float m_variaDamageReduction{};
  float m_gravityDamageReduction{};
  float m_phazonDamageReduction{};
};
class TweakPlayerRes final : public ITweak {
public:
  TweakPlayerRes(const char* ptr, std::size_t size) { delete ptr; }
};
class TweakPlayerGun final : public ITweak {
public:
  TweakPlayerGun(const char* ptr, std::size_t size) { delete ptr; }
};
class TweakPlayerControls final : public ITweak {
public:
  TweakPlayerControls(const char* ptr, std::size_t size) { delete ptr; }
};
class TweakCameraBob final : public ITweak {
public:
  TweakCameraBob(const char* ptr, std::size_t size) { delete ptr; }
};
class TweakGunRes final : public ITweak {
public:
  TweakGunRes(const char* ptr, std::size_t size) { delete ptr; }
};
class TweakParticle final : public ITweak {
public:
  TweakParticle(const char* ptr, std::size_t size) { delete ptr; }
};
class TweakSlideShow final : public ITweak {
public:
  TweakSlideShow(const char* ptr, std::size_t size) { delete ptr; }
};
class TweakAutoMapper final : public ITweak {
public:
  TweakAutoMapper(const char* ptr, std::size_t size) { delete ptr; }
};
class TweakBall final : public ITweak {
public:
  TweakBall(const char* ptr, std::size_t size) { delete ptr; }
};
class TweakGame final : public ITweak {
public:
  TweakGame(const char* ptr, std::size_t size) { delete ptr; }
};
class TweakGui final : public ITweak {
public:
  TweakGui(const char* ptr, std::size_t size) { delete ptr; }
};
class TweakGuiColors final : public ITweak {
public:
  TweakGuiColors(const char* ptr, std::size_t size) { delete ptr; }
};
class TweakTargeting final : public ITweak {
public:
  TweakTargeting(const char* ptr, std::size_t size) { delete ptr; }
};

} // namespace axdl::primedep::MetroidPrime