cmake_minimum_required(VERSION 3.5)

project(max7219-emulator LANGUAGES C)

# Src directory
add_subdirectory(src)

# Unit test module
add_subdirectory(unit_test)

add_executable(max7219-emulator main.c)
target_link_libraries(max7219-emulator)

# Enable testing
enable_testing()
add_test(NAME unit_test COMMAND unit_test)
