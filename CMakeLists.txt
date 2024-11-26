cmake_minimum_required(VERSION 2.8.8)

set(SOURCES
    src/route_engine.cpp
)

set(HEADERS
    include/route_engine.h
)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR AND NOT MSVC_IDE)
  message(FATAL_ERROR "In-source builds are not allowed.
Please create a directory and run cmake from there, passing the path to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'. Please delete them.")
endif()

if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_BUILD_TYPE Release)
endif()

project(route-engine C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(bitness 64)
  message(STATUS "Building on a 64 bit system")
else()
  message(STATUS "Building on a 32 bit system")
endif()

if(WIN32 AND MSVC_VERSION LESS 1900)
  message(FATAL_ERROR "Building with Microsoft compiler needs Latest Visual Studio 2015 (Community or better)")
endif()

link_directories(${LibOSRM_LIBRARY_DIRS})
add_executable(route-engine ${SOURCES} ${HEADERS})

find_package(LibOSRM REQUIRED)

target_link_libraries(route-engine ${LibOSRM_LIBRARIES} ${LibOSRM_DEPENDENT_LIBRARIES})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(SYSTEM ${LibOSRM_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LibOSRM_CXXFLAGS}")

find_package(fmt REQUIRED)
target_link_libraries(route-engine fmt::fmt)

find_package(PkgConfig REQUIRED)
pkg_check_modules(sigc REQUIRED sigc++-3.0)

find_package(dbus-cxx REQUIRED)
target_link_libraries(route-engine dbus-cxx::dbus-cxx)

find_package(Threads)
target_link_libraries(route-engine Threads::Threads)

target_include_directories(route-engine PRIVATE ${sigc_INCLUDE_DIRS})
