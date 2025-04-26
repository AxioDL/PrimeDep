include(FetchContent)

FetchContent_Declare(
        athena
        GIT_REPOSITORY https://github.com/libAthena/athena.git
)

FetchContent_Declare(
        nlohmann
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)

FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG v0.9.7
)

set(JSON_ImplicitConversions OFF)
FetchContent_MakeAvailable(
        athena
        nlohmann
        magic_enum
)