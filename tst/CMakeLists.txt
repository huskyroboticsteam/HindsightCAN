# Include Unity framework
add_subdirectory(unity)

# Add test runner executable
add_executable(test_runner CANCommon_test.c)

# Link Unity library
target_link_libraries(test_runner PRIVATE unity)

# Include directories for headers
target_include_directories(test_runner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/unity)

# Register the test with CTest
add_test(NAME HindsightCAN_Tests COMMAND test_runner)
