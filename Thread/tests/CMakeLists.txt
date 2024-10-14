cmake_minimum_required(VERSION 3.12)

project(Test_Thread)

set(SOURCES Thread_test.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} main.cpp)

set(BINARY_NAME tests${CMAKE_PROJECT_NAME})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_test(NAME ${BINARY_NAME} COMMAND ${BINARY_NAME})

target_link_libraries(Test_Thread PUBLIC
    GTest::gtest
    GTest::gmock
    bondevThread
)

include_directories(${CMAKE_SOURCE_DIR}/src)