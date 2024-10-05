cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(pc_gui)

set(INCLUDES
    "./include"
    "./SDL/include"
    "./SDL_ttf/include"
    "../diagnostics/include"
    "../math/include"
    "../state/include"
)

set(SOURCES
    ./src/gui_app.c
    ./src/gui_math.c
    ../math/src/general_math.c
)

set(TEST_INCLUDES
    ${INCLUDES}
)

set(TEST_SOURCES
    ${SOURCES}
    src/main.c
)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
target_link_directories(${PROJECT_NAME} PRIVATE "./SDL/build" "./SDL_ttf/build") 
target_link_libraries(${PROJECT_NAME} SDL3 SDL3_ttf m) 

add_executable("pc_gui_test" ${TEST_SOURCES})
target_include_directories("pc_gui_test" PRIVATE ${TEST_INCLUDES})
target_link_directories("pc_gui_test" PRIVATE "./SDL/build" "./SDL_ttf/build") 
target_link_libraries("pc_gui_test" SDL3 SDL3_ttf m) 

add_executable("another_test" test.c)
target_link_directories("another_test" PRIVATE "./SDL/build") 
target_link_libraries("another_test" SDL3) 
