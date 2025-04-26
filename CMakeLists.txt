cmake_minimum_required(VERSION 3.31)
project(PrimeDep)

set(CMAKE_CXX_STANDARD 23)

include(cmake/vendors.cmake)

add_library(
  PrimeDep STATIC
  src/PrimeDep/Animation/BoolPOINode.cpp
  src/PrimeDep/Animation/CharAnimTime.cpp
  src/PrimeDep/Animation/Int32POINode.cpp
  src/PrimeDep/Animation/ParticlePOINode.cpp
  src/PrimeDep/Animation/POINode.cpp
  src/PrimeDep/Animation/SoundPOINode.cpp
  src/PrimeDep/Math/AABox.cpp
  src/PrimeDep/Math/Transform4f.cpp
  src/PrimeDep/Math/Vector3f.cpp
  src/PrimeDep/Particles/ParticleData.cpp
  src/PrimeDep/ResourceNameDatabase.cpp
  src/PrimeDep/ResourcePool.cpp
  src/PrimeDep/Resources/AnimPOINodeData.cpp
  src/PrimeDep/Resources/AnimSource.cpp
  src/PrimeDep/Resources/AudioGroup.cpp
  src/PrimeDep/Resources/CharLayoutInfo.cpp
  src/PrimeDep/Resources/CollisionResponseData.cpp
  src/PrimeDep/Resources/GuiFrame.cpp
  src/PrimeDep/Resources/MapWorld.cpp
  src/PrimeDep/Resources/MetroidWorld.cpp
  src/PrimeDep/Resources/Model.cpp
  src/PrimeDep/Resources/PakFile.cpp
  src/PrimeDep/Resources/Particle.cpp
  src/PrimeDep/Resources/ParticleElectric.cpp
  src/PrimeDep/Resources/ParticleSwoosh.cpp
  src/PrimeDep/Resources/ProjectileWeapon.cpp
  src/PrimeDep/Resources/RasterFont.cpp
  src/PrimeDep/Resources/ScannableObjectInfo.cpp
  src/PrimeDep/Resources/SkinRules.cpp
  src/PrimeDep/Resources/StringTable.cpp
  src/PrimeDep/Resources/Texture.cpp)

target_include_directories(PrimeDep PUBLIC include)
target_link_libraries(PrimeDep PUBLIC athena-core nlohmann_json::nlohmann_json
                                      magic_enum)

add_executable(test main.cpp)
target_link_libraries(test PrimeDep)
target_compile_options(PrimeDep PRIVATE -Wno-multichar -Wno-ignored-attributes)

target_compile_options(test PRIVATE -Wno-multichar -Wno-ignored-attributes)
