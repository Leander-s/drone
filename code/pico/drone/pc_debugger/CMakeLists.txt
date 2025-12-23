cmake_minimum_required(VERSION 3.13)

project(drone_debugger C)

set(HEADERS
    src/connection.h
    src/config.h
)

set(SOURCES
    src/connection.c
    src/main.c
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

#[[install(TARGETS ${PROJECT_NAME} 
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

install(TARGETS "ground_protocol_test" RUNTIME DESTINATION bin)
]]#
