# Add test runner executable, including Unity source
add_executable(test_runner
    unity/unity.c
    CANCommon_test.c
)

# Include Unity headers
target_include_directories(test_runner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/unity
)

# Link the main library under test
target_link_libraries(test_runner PRIVATE HindsightCAN)

# Register the test with CTest
add_test(NAME HindsightCAN_Tests COMMAND test_runner)
