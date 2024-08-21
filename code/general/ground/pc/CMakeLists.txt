cmake_minimum_required(VERSION 3.10)

set(LIBRARY_DIR "./lib")
set(INCLUDE_DIR "./include")
file(GLOB LIBRARY_FILES "${LIBRARY_DIR}/*.a")

file(GLOB SOURCES
    "./src/*.c"
)

file(GLOB HEADERS
    "./src/*.h"
)

project(pc_app C)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_FILES})
target_include_directories(${PROJECT_NAME} ${INCLUDE_DIR})
