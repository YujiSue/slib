cmake_minimum_required(VERSION 3.14)
project(slib)
option(BUILD_SOBJ_LIB "build sobj library" ON) 
option(BUILD_SAPP_LIB "build sapp library" OFF) 
option(BUILD_SNAPP_LIB "build node native plugin library" OFF) 
option(BUILD_BIOINFO_LIB "build sbioinfo library" OFF) 
option(BUILD_SHARED_LIBS "build slib as a shared library" OFF)
option(BUILD_ONLY "build but not install lib(s)" OFF)
option(BUILD_TEST_APP "build test app" OFF)
if (BUILD_SHARED_LIBS)
  set(BUILD_MODE SHARED)
else()
  set(BUILD_MODE STATIC)
endif()
message(STATUS "BUILD_MODE=${BUILD_MODE}")
message(STATUS "INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include")
message(STATUS "LIB_DIR=$ENV{LD_LIBRARY_PATH}")
message(STATUS "BUILD_BIOINFO_LIB=${BUILD_BIOINFO_LIB}")
message(STATUS "BUILD_SAPP_LIB=${BUILD_SAPP_LIB}")
message(STATUS "BUILD_SNAPP_LIB=${BUILD_SNAPP_LIB}")
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories($ENV{LD_LIBRARY_PATH})
if (BUILD_ONLY)
set(CMAKE_INSTALL_PREFIX ".")
endif()
add_subdirectory(src)
if (BUILD_TEST_APP)
  add_subdirectory(test)
endif()
