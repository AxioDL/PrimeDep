#include "PrimeDep/Resources/MetroidPrime/Tweak.hpp"

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

} // namespace axdl::primedep::MetroidPrime