cmake_minimum_required(VERSION 3.31)
project(PrimeDep)

set(CMAKE_CXX_STANDARD 23)

include(cmake/vendors.cmake)

add_library(PrimeDep STATIC
        src/PrimeDep/Resources/PakFile.cpp
        src/PrimeDep/Resources/Texture.cpp
        src/PrimeDep/Resources/AudioGroup.cpp
        src/PrimeDep/Resources/MetroidWorld.cpp
        src/PrimeDep/Math/Transform4f.cpp
        src/PrimeDep/Math/AABox.cpp
        src/PrimeDep/Math/Vector3f.cpp
        src/PrimeDep/ResourceNameDatabase.cpp
        src/PrimeDep/ResourcePool.cpp
)

target_include_directories(PrimeDep PUBLIC include)
target_link_libraries(PrimeDep PUBLIC
        athena-core
        nlohmann_json::nlohmann_json
        magic_enum
)

add_executable(test main.cpp)
target_link_libraries(test
        PrimeDep
)