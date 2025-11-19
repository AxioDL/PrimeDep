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
  TweakPlayer(const char* ptr, std::size_t size) { delete ptr; }
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