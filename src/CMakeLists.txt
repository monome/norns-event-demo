cmake_minimum_required(VERSION 3.2)

project(event_demo CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

include(FindPkgConfig)
pkg_check_modules(LIBLUA REQUIRED lua-5.3)
include_directories(${LIBLUA_INCLUDE_DIRS})

set(NORNS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/dep/norns")

include_directories("${NORNS_SOURCE_DIR}/matron/src")
include_directories("${NORNS_SOURCE_DIR}/source")

set(SOURCE_FILES
  lua_event_demo.cpp
)

add_library(event_demo MODULE ${SOURCE_FILES})
set_target_properties(event_demo PROPERTIES PREFIX "") # string `lib` prefix
target_link_libraries(event_demo pthread)
