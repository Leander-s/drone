cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LIBRARY_DIR "./lib")
set(INCLUDE_DIR 
    "./include"
    "../../communications/ground/pc_side/include"
    "../../math/include"
    "../../state/include"
    "../../debugging_util/include"
    "../../diagnostics/include"
    "../../gui/include"
    "../../gui/SDL/include"
    "../../gui/SDL_ttf/include"
    "../../communications/protocol_util/include"
)
set(SOURCE_DIR
    "./src"
    "../../communications/ground/pc_side/src"
    "../../communications/protocol_util/src"
    "../../math/src"
    "../../gui/src"
    "../../state/src"
    "../../debugging_util/src"
    "../../diagnostics/src"
)

set(SOURCES
    src/main.c
)

foreach(DIR ${SOURCE_DIR})
    file(GLOB FILE
        "${DIR}/*.c" EXCEPT "*/main.c" "*/test*.c"
    )
    list(APPEND SOURCES ${FILE})
endforeach()

message("Sources : ${SOURCES}")

project(pc_app C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_directories(${PROJECT_NAME} PRIVATE "../../gui/SDL/build" "../../gui/SDL_ttf/build")
target_link_libraries(${PROJECT_NAME} ${LIBRARY_FILES} SDL3 SDL3_ttf m)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR} )
