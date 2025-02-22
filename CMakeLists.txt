cmake_minimum_required(VERSION 3.24)
project(SandSim)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS FALSE)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML.git
        GIT_TAG         2.5.1
)

FetchContent_MakeAvailable(SFML)
add_executable(Main main.cpp)
target_compile_definitions(Main PRIVATE SFML_BUILD_STATIC)
target_link_libraries(Main sfml-graphics sfml-audio sfml-network sfml-window sfml-system)
