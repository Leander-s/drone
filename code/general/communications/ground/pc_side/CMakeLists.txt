cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ground_protocol_pc)

set(COMMUNICATIONS_HEADERS
    include/protocol.h
    include/connection.h
    include/ground_protocol_config.h
    ../../protocol_util/protocol_util.h
    ../../../debugging_util/diagnostics.h
    ../../../state/drone_states.h
)

set(COMMUNICATIONS_SOURCES
    src/protocol.c
    src/connection.c
    ../../protocol_util/protocol_util.c
    ../../../debugging_util/diagnostics.c
)

set(COMMUNICATIONS_TEST_HEADERS
    include/protocol.h
    include/test_config.h
    include/connection.h
    include/util.h
    include/input_poll.h
    ../../protocol_util/protocol_util.h
    ../../../debugging_util/diagnostics.h
    ../../../state/drone_states.h
)

set(COMMUNICATIONS_TEST_SOURCES
    src/util.c
    src/connection.c
    src/test_protocol.c
    src/input_poll.c
    src/main.c
    ../../protocol_util/protocol_util.c
    ../../../debugging_util/diagnostics.c
)

add_library(${PROJECT_NAME} ${COMMUNICATIONS_SOURCES} ${COMMUNICATIONS_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE "../../../state/" "../../../debugging_util/" "../../protocol_util" "./include") 

add_executable("ground_protocol_pc_test" ${COMMUNICATIONS_TEST_SOURCES} ${COMMUNICATIONS_TEST_HEADERS})
target_include_directories("ground_protocol_pc_test" PRIVATE "../../../state/" "../../../debugging_util/" "../../protocol_util" "./include") 
if(UNIX)
target_link_libraries("ground_protocol_pc_test" X11)
endif(UNIX)
