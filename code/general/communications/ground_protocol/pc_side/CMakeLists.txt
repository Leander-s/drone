cmake_minimum_required(VERSION 3.13)

project(ground_protocol_pc)

set(COMMUNICATIONS_HEADERS
    src/util.h
    src/protocol.h
    src/input_poll.h
    src/connection.h
    src/ground_protocol_config.h
)

set(COMMUNICATIONS_SOURCES
    src/util.c
    src/protocol.c
    src/input_poll.c
    src/connection.c
)

set(COMMUNICATIONS_TEST_HEADERS
    ${COMMUNICATIONS_HEADERS}
)

set(COMMUNICATIONS_TEST_SOURCES
    ${COMMUNICATIONS_SOURCES}
    src/main.c
)

add_library(${PROJECT_NAME} ${COMMUNICATIONS_SOURCES} ${COMMUNICATIONS_HEADERS})
target_link_libraries(${PROJECT_NAME} X11)

add_executable("ground_protocol_pc_test" ${COMMUNICATIONS_TEST_SOURCES} ${COMMUNICATIONS_TEST_HEADERS})
target_link_libraries("ground_protocol_pc_test" X11)

#[[install(TARGETS ${PROJECT_NAME} 
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

install(TARGETS "ground_protocol_test" RUNTIME DESTINATION bin)
]]#
