cmake_minimum_required(VERSION 3.17)
project(TD1_simulateur C)

set(CMAKE_C_STANDARD 11)

add_executable(VMINC
        VMinC.c)
add_executable(ISS ISS.c)
