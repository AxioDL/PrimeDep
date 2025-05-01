add_executable(packer src/main.cpp src/PackagePrime.cpp src/PackagePrime.cpp)
target_link_libraries(packer PRIVATE PrimeDep argparse::argparse)
target_include_directories(packer PRIVATE include)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(packer PRIVATE -Wno-multichar -Wno-ignored-attributes)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(packer PRIVATE -Wno-multichar -Wno-ignored-attributes)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_definitions(packer PRIVATE _CRT_SECURE_NO_DEPRECATE=1
                                            _CRT_SECURE_NO_WARNINGS=1)
endif()
