# Unity framework
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/unity_framework)
add_subdirectory(unity_framework)

add_executable(unit_test ut.h ut_runner.c)
target_link_libraries(unit_test unity_framework)
