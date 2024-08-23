cmake_minimum_required(VERSION 3.10)

project(ground_protocol_pc)

set(HEADERS
    src/util.h
    src/config.h
)

set(SOURCE
    src/util.c
    src/main.c
)

add_library(${PROJECT_NAME} ${COMMUNICATIONS_SOURCES} ${COMMUNICATIONS_HEADERS})
target_link_directories(${CMAKE_LINK_DIRECTORIES_BEFORE} "./SDL/build") 
target_link_libraries(${PROJECT_NAME} SDL3) 

add_executable("pc_gui_test" ${COMMUNICATIONS_TEST_SOURCES} ${COMMUNICATIONS_TEST_HEADERS})
target_link_directories(${CMAKE_LINK_DIRECTORIES_BEFORE} "./SDL/build") 
target_link_libraries(${PROJECT_NAME} SDL3) 
