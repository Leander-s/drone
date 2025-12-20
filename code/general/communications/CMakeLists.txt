cmake_minimum_required(VERSION 3.20)
project(drone_system LANGUAGES C)

add_subdirectory(nrf24)
add_subdirectory(protocol_util)
add_subdirectory(drone)
add_subdirectory(ground)
