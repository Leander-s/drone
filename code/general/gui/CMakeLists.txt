cmake_minimum_required(VERSION 3.10)

project(ground_protocol_pc)

set(HEADERS
    src/config.h
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
target_link_directories(${PROJECT_NAME} PRIVATE "./SDL/build") 
target_link_libraries(${PROJECT_NAME} SDL3) 

add_executable("pc_gui_test" ${TEST_SOURCES} ${TEST_HEADERS})
target_link_directories(${PROJECT_NAME} PRIVATE "./SDL/build") 
target_link_libraries(${PROJECT_NAME} SDL3) 
