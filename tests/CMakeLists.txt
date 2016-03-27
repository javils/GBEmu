# Initialize testing framework CTest.
find_package(CppUTest REQUIRED)
enable_testing()

# Add all .c, .cpp and .h here
set(TEST_SRC
        GBTests.cpp
        CPUTests.cpp
        ByteLoadOpTests.cpp
        CPUControlTests.cpp
        WordLoadOpTests.cpp
        WordLogicalTests.cpp
        ByteLogicalTests.cpp
        RotateShiftTests.cpp
        JumTests.cpp)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lCppUTest" )

# Add include of library
include_directories(${GLOBAL_INC_FILES})

# Compile GB_tests
add_executable(GB_tests ${TEST_SRC})
target_link_libraries(GB_tests GBEmu)

# Add test and make new command "make utest" that pass all the test.
add_test(GB_tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/GB_tests)
add_custom_target(utest COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/GB_tests
        DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/GB_tests)