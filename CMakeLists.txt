cmake_minimum_required(VERSION 3.5)

project(max7219-emulator LANGUAGES C)

# Unit test module
add_subdirectory(unit_test)

add_executable(max7219-emulator main.c)

# Enable testing
enable_testing()
add_test(NAME unit_test COMMAND unit_test)
