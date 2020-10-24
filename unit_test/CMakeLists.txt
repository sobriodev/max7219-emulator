# Unity framework
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/unity_framework)
add_subdirectory(unity_framework)

# Src
include_directories(${max7219-emulator_SOURCE_DIR}/src)

add_executable(unit_test
    ut.h
    ut_runner.c
    ut_handle.c)

target_link_libraries(unit_test src unity_framework)
