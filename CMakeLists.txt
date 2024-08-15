cmake_minimum_required(VERSION 3.29.2)

project(
  SKI
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# gtest start ###
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# gtest start ###

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(tokenizer OBJECT ski/tokenizer.cc)
add_library(parser OBJECT ski/parser.cc)
add_library(interpreter OBJECT ski/interpreter.cc)

add_executable(ski ski/main.cc $<TARGET_OBJECTS:tokenizer>
                   $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:interpreter>)

enable_testing()

add_executable(tokenizer_test EXCLUDE_FROM_ALL test/tokenizer_test.cc
                                               $<TARGET_OBJECTS:tokenizer>)
target_link_libraries(tokenizer_test PRIVATE GTest::gtest_main)

add_executable(
  parser_test EXCLUDE_FROM_ALL test/parser_test.cc $<TARGET_OBJECTS:tokenizer>
                               $<TARGET_OBJECTS:parser>)
target_link_libraries(parser_test PRIVATE GTest::gtest_main)

add_executable(
  interpreter_test EXCLUDE_FROM_ALL
  test/interpreter_test.cc $<TARGET_OBJECTS:tokenizer> $<TARGET_OBJECTS:parser>
  $<TARGET_OBJECTS:interpreter>)
target_link_libraries(interpreter_test PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tokenizer_test)
gtest_discover_tests(parser_test)
gtest_discover_tests(interpreter_test)
