cmake_minimum_required(VERSION 3.31)
project(PrimeDep)

set(CMAKE_CXX_STANDARD 23)

include(cmake/vendors.cmake)

add_library(PrimeDep STATIC
        src/PrimeDep/Resources/PakFile.cpp
        include/PrimeDep/Resources/AssetId.hpp
        include/PrimeDep/Resources/ResourceFactory.hpp
        src/PrimeDep/Resources/Texture.cpp
        src/PrimeDep/Resources/AudioGroup.cpp
        include/PrimeDep/Resources/AudioGroup.hpp
)

target_include_directories(PrimeDep PUBLIC include)
target_link_libraries(PrimeDep PUBLIC
        athena-core
        nlohmann_json::nlohmann_json
)

add_executable(test main.cpp)
target_link_libraries(test
        PrimeDep
)