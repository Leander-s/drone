cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(pc_gui)

set(HEADERS
    src/gui_config.h
    src/gui_app.h
)

set(SOURCES
    src/gui_app.c
)

set(TEST_HEADERS
    ${HEADERS}
)

set(TEST_SOURCES
    ${SOURCES}
    src/main.c
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE "./SDL_ttf/include" "../debugging_util")
target_link_directories(${PROJECT_NAME} PRIVATE "./SDL/build" "./SDL_ttf/build") 
target_link_libraries(${PROJECT_NAME} SDL3 SDL3_ttf) 

add_executable("pc_gui_test" ${TEST_SOURCES} ${TEST_HEADERS})
target_include_directories("pc_gui_test" PRIVATE "./SDL_ttf/include" "../debugging_util")
target_link_directories("pc_gui_test" PRIVATE "./SDL/build" "./SDL_ttf/build") 
target_link_libraries("pc_gui_test" SDL3 SDL3_ttf) 
