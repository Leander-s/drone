cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(INCLUDE_DIR 
    "./src/communications"
    "./src/gui"
    "./src/gui/SDL/include"
    "./src/gui/SDL_ttf/include"
    "./src/gui/cglm/include"
    "../general/encoding/include"
    "../general/math/include"
    "../general/state/include"
    "../general/diagnostics/include"
)
set(SOURCE_DIR
    "./src"
    "./src/gui"
    "./src/communications"
    "../general/encoding/src"
    "../general/math/src"
    "../general/state/src"
    "../general/diagnostics/src"
)

foreach(DIR ${SOURCE_DIR})
    file(GLOB FILE
        "${DIR}/*.c"
    )
    list(APPEND SOURCES ${FILE})
    list(FILTER SOURCES EXCLUDE REGEX "main.c")
    list(FILTER SOURCES EXCLUDE REGEX "test.c")
endforeach()
list(APPEND SOURCES src/main.c)

project(pc_app C)

set(SDLTTF_VENDORED ON)

add_subdirectory("./src/gui/SDL" EXCLUDE_FROM_ALL)
add_subdirectory("./src/gui/SDL_ttf" EXCLUDE_FROM_ALL)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} GL SDL3::SDL3 SDL3_ttf::SDL3_ttf m udev)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR} )
