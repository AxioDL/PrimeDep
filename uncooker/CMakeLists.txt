
add_executable(uncooker src/main.cpp)
target_link_libraries(uncooker PRIVATE PrimeDep)

target_compile_options(uncooker PRIVATE -Wno-multichar -Wno-ignored-attributes)