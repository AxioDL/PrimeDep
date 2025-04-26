add_executable(uncooker src/main.cpp)
target_link_libraries(uncooker PRIVATE PrimeDep)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(uncooker PRIVATE -Wno-multichar
                                          -Wno-ignored-attributes)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(uncooker PRIVATE -Wno-multichar
                                          -Wno-ignored-attributes)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_definitions(uncooker PRIVATE -D_CRT_SECURE_NO_DEPRECATE=1
                                              _D_CRT_SECURE_NO_WARNINGS=1)
endif()
