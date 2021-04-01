include(FindGLEW)
include(FindOpenGL)
include(FindLua)
include(FindZLIB)
include(FindSDL2)
include(FindSDL2_mixer)
include(Findassimp)
include(FindFreetype)
include(FindFreetype-gl)
include(glmConfig)

message(IMGUI_2=${IMGUI_SOURCES})
add_executable(needle_game ${SOURCES} ${IMGUI_SOURCES})

#GLVND / LEGACY
set(OpenGL_GL_PREFERENCE "GLVND")

find_package(GLEW REQUIRED)
if(GLEW_FOUND)
        include_directories(${GLEW_INCLUDE_DIRS})                  
        target_link_libraries (needle_game ${GLEW_LIBRARIES})      
endif(GLEW_FOUND)
message(GLEW_FOUND=${GLEW_FOUND})
message(GLEW_VERSION=${GLEW_VERSION})

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
        include_directories(${OPENGL_INCLUDE_DIR})                  
        target_link_libraries (needle_game ${OPENGL_LIBRARIES})      
endif(OPENGL_FOUND)
message(OPENGL_FOUND=${OPENGL_FOUND})

find_package(Lua REQUIRED)
if(LUA_FOUND)
        include_directories(${LUA_INCLUDE_DIR})                  
        target_link_libraries (needle_game ${LUA_LIBRARIES})      
endif(LUA_FOUND)
message(LUA_FOUND=${LUA_FOUND})
message(LUA_VERSION=${LUA_VERSION_STRING})

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
        include_directories(${SDL2_INCLUDE_DIRS})                  
        target_link_libraries (needle_game ${SDL2_LIBRARIES})      
endif(SDL2_FOUND)
message(SDL2_FOUND=${SDL2_FOUND})
message(SDL2_VERSION=${SDL2_VERSION_STRING})

find_package(SDL2_mixer REQUIRED)
if(SDL2_MIXER_FOUND)
        include_directories(${SDL2_MIXER_INCLUDE_DIRS})                  
        target_link_libraries (needle_game ${SDL2_MIXER_LIBRARIES})      
endif(SDL2_MIXER_FOUND)
message(SDL2_MIXER_FOUND=${SDL2_MIXER_FOUND})
message(SDL2_MIXER_VERSION=${SDL2_MIXER_VERSION_STRING})

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
        include_directories(${ZLIB_INCLUDE_DIRS})                  
        target_link_libraries (needle_game ${ZLIB_LIBRARIES})      
endif(ZLIB_FOUND)
message(ZLIB_FOUND=${ZLIB_FOUND})
message(ZLIB_VERSION=${ZLIB_VERSION_STRING})

find_package(ASSIMP REQUIRED)
if(ASSIMP_FOUND)
        include_directories(${ASSIMP_INCLUDE_DIR})
        target_link_libraries (needle_game ${ASSIMP_LIBRARIES})  
endif(ASSIMP_FOUND)
message(ASSIMP_FOUND=${ASSIMP_FOUND})

find_package(Freetype REQUIRED)
if(FREETYPE_FOUND)
        include_directories(${FREETYPE_INCLUDE_DIRS})
        target_link_libraries (needle_game ${FREETYPE_LIBRARIES})  
endif(FREETYPE_FOUND)
message(FREETYPE_FOUND=${FREETYPE_FOUND})
message(FREETYPE_VERSION = ${FREETYPE_VERSION_STRING})

find_package(Freetype-gl REQUIRED)
if(FREETYPE_GL_FOUND)
        include_directories(${FREETYPE_GL_INCLUDE_DIR})
        target_link_libraries (needle_game ${FREETYPE_GL_LIBRARIES})  
endif(FREETYPE_GL_FOUND)
message(FREETYPE_GL_FOUND=${FREETYPE_GL_FOUND})


add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
message(${GLM_INCLUDE_DIRS})
#target_link_libraries (needle_game ${GLM_LIBRARIES})  
message(GLM_FOUND=${GLM_FOUND})
message(GLM_VERSION = ${GLM_VERSION})

#LIBRARY FLAGS
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lz -lGLEW -lSDL2 -lSDL2main")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

#COMPILE OPTIONS
target_compile_options(needle_game PRIVATE
        -lstdc++fs
        -msse2
        -msse
        )

target_compile_features(needle_game PUBLIC cxx_std_20)