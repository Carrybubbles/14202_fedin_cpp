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

add_executable(xtest "test.cpp" "noisy.cpp")
target_compile_options(xtest PUBLIC  -fprofile-arcs -ftest-coverage -fPIC -g -c -Wnon-template-friend -O0 -Wall -pedantic -std=c++1z)
add_test(NAME test COMMAND xtest)
target_link_libraries(xtest gtest gmock_main  --coverage pthread)

add_executable(range_view "main.cpp" "rangeview.h" "noisy.cpp")
target_compile_options(range_view PUBLIC -fprofile-arcs -ftest-coverage -fPIC -g -Wnon-template-friend  -c -O0 -Wall -pedantic -std=c++1z)
target_link_libraries(range_view --coverage)
