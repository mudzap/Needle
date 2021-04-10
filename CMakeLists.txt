cmake_minimum_required(VERSION 3.11) #for GLVND/LEGACY OpenGL
project(needle_game)

include(FindPkgConfig)

#MODULES
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/assimp)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/freetype-gl)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/glm)

set(CMAKE_BUILD_TYPE Debug)

include_directories(include
                    src
                    )
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

include_directories(include/imgui)
file(GLOB IMGUI_SOURCES "include/imgui/*.cpp")
file(GLOB_RECURSE IMGUI_SOURCES_2 "include/imgui/misc/*.cpp")
list(APPEND IMGUI_SOURCES ${IMGUI_SOURCES_2})

#include_directories(include/glm)

#SOURCE CODE
add_subdirectory(src)

#EXECUTABLES
add_subdirectory(app)

#ASSETS
add_subdirectory(assets)

#DOXYGEN
#add_subdirectory(docs)
