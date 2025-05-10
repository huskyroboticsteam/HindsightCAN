# Define the list of test source files
set(TEST_SOURCES
    CANCommon_test.c
    CANCommon_test1.c
    # ADD TEST FILES HERE
)

# Include Unity headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/unity)

# Iterate over each test source file
foreach(test_src ${TEST_SOURCES})
    # Extract the base name without extension
    get_filename_component(test_name ${test_src} NAME_WE)

    # Define the test executable name
    set(test_exe "${test_name}")

    # Create the test executable
    add_executable(${test_exe}
        unity/unity.c
        ${test_src}
    )

    # Link the main library under test
    target_link_libraries(${test_exe} PRIVATE HindsightCAN)

    # Register the test with CTest
    add_test(NAME ${test_exe} COMMAND ${test_exe})
endforeach()
