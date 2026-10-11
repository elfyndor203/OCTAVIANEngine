if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.15)
    set(CMAKE_C_STANDARD 99)
    project(OCTAVIANEngine C)
endif()

add_subdirectory(External)

add_subdirectory(Core)

add_subdirectory(_SystemEx)
add_subdirectory(World)
add_subdirectory(Renderer)
add_subdirectory(Input)
add_subdirectory(Platform)
add_subdirectory(Physics)
add_subdirectory(OCTAVIANEngine)
