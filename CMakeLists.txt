cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_STANDARD 99)

project(ISS C)

# Copying binary file to debug folder
configure_file(bin/instruction.bin bin/instruction.bin COPYONLY)

add_executable(ISS
        src/ISS.c)