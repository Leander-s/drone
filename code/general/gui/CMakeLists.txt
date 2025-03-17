cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLEW_INCLUDE_DIRS /usr/include/GL)
set(GLEW_LIBRARIES /usr/lib/x86_64-linux-gnu/libGLEW.so)

find_package(SDL3 REQUIRED)

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
target_link_directories(${PROJECT_NAME} PRIVATE "./SDL/build" "./SDL_ttf/build" ${GLEW_INCLUDE_DIRS}) 
target_link_libraries(${PROJECT_NAME} GLEW GL SDL3 SDL3_ttf m cglm) 

add_executable("pc_gui_test" ${TEST_SOURCES})
target_include_directories("pc_gui_test" PRIVATE ${TEST_INCLUDES})
target_link_directories("pc_gui_test" PRIVATE "./SDL/build" "./SDL_ttf/build" ${GLEW_INCLUDE_DIRS}) 
target_link_libraries("pc_gui_test" GLEW GL SDL3 SDL3_ttf m cglm) 

add_executable("another_test" test.c)
target_link_directories("another_test" PRIVATE "./SDL/build") 
target_link_libraries("another_test" SDL3) 
