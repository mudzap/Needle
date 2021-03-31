add_executable(needle_game ${SOURCES})

find_library(BS2B NAMES libbs2b.a)
message(${BS2B})



find_library(SDL2 NAMES libSDL2main.a)
message(${SDL2})

target_compile_options(needle_game PRIVATE -lstdc++fs)

target_include_directories(needle_game PRIVATE
        ${SDL2}
        )

target_link_libraries (needle_game
        ${SDL2}
        )