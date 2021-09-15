cmake_minimum_required(VERSION 3.2)

set(LUA_MODULE_NAME "event_demo")

project(lua-module CXX)

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

add_library(lua-module MODULE ${SOURCE_FILES})
set_target_properties(lua-module PROPERTIES
  PREFIX ""     # strip `lib` prefix
  OUTPUT_NAME "${LUA_MODULE_NAME}"
)
target_link_libraries(lua-module pthread)
