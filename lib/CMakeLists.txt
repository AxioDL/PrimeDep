add_library(
  PrimeDep STATIC
  src/PrimeDep/Animation/BoolPOINode.cpp
  src/PrimeDep/Animation/CharAnimTime.cpp
  src/PrimeDep/Animation/Int32POINode.cpp
  src/PrimeDep/Animation/ParticlePOINode.cpp
  src/PrimeDep/Animation/POINode.cpp
  src/PrimeDep/Animation/SoundPOINode.cpp
  src/PrimeDep/AssetId.cpp
  src/PrimeDep/CRC32.cpp
  src/PrimeDep/Math/AABox.cpp
  src/PrimeDep/Math/Transform4f.cpp
  src/PrimeDep/Math/Vector2i.cpp
  src/PrimeDep/Math/Vector3f.cpp
  src/PrimeDep/ObjectTag.cpp
  src/PrimeDep/Particles/ParticleData.cpp
  src/PrimeDep/ResourceNameDatabase.cpp
  src/PrimeDep/ResourcePool.cpp
  src/PrimeDep/Resources/MetroidPrime/AiFiniteStateMachine.cpp
  src/PrimeDep/Resources/MetroidPrime/AnimCharacterSet.cpp
  src/PrimeDep/Resources/MetroidPrime/AnimPOINodeData.cpp
  src/PrimeDep/Resources/MetroidPrime/AnimSource.cpp
  src/PrimeDep/Resources/MetroidPrime/AudioGroup.cpp
  src/PrimeDep/Resources/MetroidPrime/AudioTranslationTable.cpp
  src/PrimeDep/Resources/MetroidPrime/CharLayoutInfo.cpp
  src/PrimeDep/Resources/MetroidPrime/CollidableOBBTreeGroup.cpp
  src/PrimeDep/Resources/MetroidPrime/CollisionResponseData.cpp
  src/PrimeDep/Resources/MetroidPrime/DecalData.cpp
  src/PrimeDep/Resources/MetroidPrime/DependencyGroup.cpp
  src/PrimeDep/Resources/MetroidPrime/GuiFrame.cpp
  src/PrimeDep/Resources/MetroidPrime/MapArea.cpp
  src/PrimeDep/Resources/MetroidPrime/MapUniverse.cpp
  src/PrimeDep/Resources/MetroidPrime/MapWorld.cpp
  src/PrimeDep/Resources/MetroidPrime/MetroidArea.cpp
  src/PrimeDep/Resources/MetroidPrime/MetroidWorld.cpp
  src/PrimeDep/Resources/MetroidPrime/MidiData.cpp
  src/PrimeDep/Resources/MetroidPrime/Model.cpp
  src/PrimeDep/Resources/MetroidPrime/PakFile.cpp
  src/PrimeDep/Resources/MetroidPrime/Particle.cpp
  src/PrimeDep/Resources/MetroidPrime/ParticleElectric.cpp
  src/PrimeDep/Resources/MetroidPrime/ParticleSwoosh.cpp
  src/PrimeDep/Resources/MetroidPrime/PathFindArea.cpp
  src/PrimeDep/Resources/MetroidPrime/ProjectileWeapon.cpp
  src/PrimeDep/Resources/MetroidPrime/RasterFont.cpp
  src/PrimeDep/Resources/MetroidPrime/SaveWorld.cpp
  src/PrimeDep/Resources/MetroidPrime/ScannableObjectInfo.cpp
  src/PrimeDep/Resources/MetroidPrime/SkinRules.cpp
  src/PrimeDep/Resources/MetroidPrime/StringTable.cpp
  src/PrimeDep/Resources/MetroidPrime/Texture.cpp
  src/PrimeDep/WorldCommon/MetroidPrime/MappableObject.cpp)

target_include_directories(PrimeDep PUBLIC include ${tinygltf_SOURCE_DIR})
target_link_libraries(PrimeDep PUBLIC athena-core nlohmann_json::nlohmann_json
                                      magic_enum tinygltf)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(PrimeDep PRIVATE -Wno-multichar
                                          -Wno-ignored-attributes)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(PrimeDep PRIVATE -Wno-multichar
                                          -Wno-ignored-attributes)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_definitions(PrimeDep PRIVATE _CRT_SECURE_NO_DEPRECATE=1
                                              _CRT_SECURE_NO_WARNINGS=1)
endif()
