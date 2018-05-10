cmake_minimum_required(VERSION 2.8)
project(rangeview)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")

include(ExternalProject)
ExternalProject_Add( "googletest"
                        GIT_REPOSITORY "https://github.com/google/googletest.git"
                        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/deps"
)
include_directories("${PROJECT_SOURCE_DIR}/deps/include")
link_directories("${PROJECT_SOURCE_DIR}/deps/lib/")

add_executable(xtest "main.cpp" "rangeview.h")
target_compile_options(xtest PUBLIC  -fprofile-arcs -ftest-coverage -fPIC -g -c -O0 -Wall -pedantic -std=c++11)
add_test(NAME xor_test COMMAND xtest)
target_link_libraries(xtest gtest gmock_main  --coverage pthread)

add_executable(range_view "main.cpp" "rangeview.h" )
target_compile_options(range_view PUBLIC -fprofile-arcs -ftest-coverage -fPIC -g  -c -O0 -Wall -pedantic -std=c++11)
target_link_libraries(range_view --coverage)
