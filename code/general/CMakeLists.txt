cmake_minimum_required(VERSION 3.20)
project(drone_system LANGUAGES C)

add_subdirectory(math)
add_subdirectory(debugging_utils)
add_subdirectory(state)
add_subdirectory(diagnostics)
add_subdirectory(sensors)
add_subdirectory(controls)
add_subdirectory(communications)
add_subdirectory(gui)

add_subdirectory(ground)
add_subdirectory(drone)
