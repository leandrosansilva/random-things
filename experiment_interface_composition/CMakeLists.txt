project(chuecken)

cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

set(DEPS_INSTALL_DIR ${CMAKE_SOURCE_DIR}/3rd)

enable_testing()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${DEPS_INSTALL_DIR}/ctti/include/ 
  ${DEPS_INSTALL_DIR}/brigand/include/ 
  ${DEPS_INSTALL_DIR}/mpl/src/
)

add_definitions(-Wall -Wextra -Wpedantic)

add_executable(tests implementor_tests.cpp)
add_executable(chuck chuck_norris.cpp)
