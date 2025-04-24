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

set(JSON_ImplicitConversions OFF)
FetchContent_MakeAvailable(
        athena
        nlohmann
)