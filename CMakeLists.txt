cmake_minimum_required(VERSION 3.10)
project(needle_game)

include(FindPkgConfig)
include(CTest)

set(CMAKE_BUILD_TYPE Debug)

include_directories(include
                    src
                    )
#set(SOURCES src/main.cpp)
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

#SOURCE CODE
add_subdirectory(src)

#EXECUTABLES
add_subdirectory(app)

#ASSETS
add_subdirectory(assets)

#TESTING
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()