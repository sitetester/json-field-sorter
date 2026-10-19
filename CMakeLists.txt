cmake_minimum_required(VERSION 3.25)
project(json_field_sorter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(json_field_sorter
    main.cpp
    token_sorter.cpp
)

target_include_directories(json_field_sorter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
enable_testing()

add_executable(test_token_sorter
    tests/test_token_sorter.cpp
    token_sorter.cpp
)

target_include_directories(test_token_sorter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_token_sorter PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_token_sorter)
