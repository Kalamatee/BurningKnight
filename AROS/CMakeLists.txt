cmake_minimum_required(VERSION 3.16)
project(BurningKnightAROS C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

add_executable(bk_aros
    src/main.c
    src/game.c
    src/platform.c
    src/entity/entity_manager.c
    src/level/level_manager.c
    src/state/state_manager.c
    src/ui/ui_manager.c
    src/util/log.c
    src/core/events.c
    src/core/save.c
    src/core/settings.c
    src/core/tags.c
)

target_include_directories(bk_aros PRIVATE
    include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(bk_aros PRIVATE ${SDL2_LIBRARIES})
