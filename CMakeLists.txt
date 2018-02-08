cmake_minimum_required(VERSION 3.2)

project(snapcaster-ui LANGUAGES CXX VERSION 0.1.0)
set(PROJECT_DESCRIPTION "User interface for the snapcast audio player")
set(PROJECT_URL "https://github.com/laszloh/snapcaster-ui")

option(BUILD_SHARED_LIBS "Build snapcaster-ui in a shared context" ON)
option(BUILD_STATIC_LIBS "Build snapcaster-ui in a static context" ON)
option(BUILD_TESTS "Build tests (run tests with make test)" ON)

if (NOT BUILD_SHARED_LIBS AND NOT BUILD_STATIC_LIBS)
    message(FATAL_ERROR "One or both of BUILD_SHARED_LIBS or BUILD_STATIC_LIBS"
                        "must be set to ON to build")
endif()

# Configure paths
if(NOT DEFINED CMAKE_INSTALL_BINDIR)
    SET(CMAKE_INSTALL_BINDIR bin CACHE
    PATH "Output directory for binary files")
endif()

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
    SET(CMAKE_INSTALL_LIBDIR lib CACHE PATH "Output directory for libraries")
endif()

if(NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
    SET(CMAKE_INSTALL_INCLUDEDIR include CACHE
        PATH "Output directory for header files")
endif()

include_directories("${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/externals/popl/include")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
add_definitions(-DVERSION="${PROJECT_VERSION}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Configure compiler options
set(CMAKE_CXX_STANDARD 11)

# Get arch
include(${CMAKE_SOURCE_DIR}/cmake/TargetArch.cmake)
target_architecture(ARCH)

#set(THREADS_PTHREAD_ARG "2" CACHE STRING "Forcibly set by CMakeLists.txt." FORCE)

# Check dependencies
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

include(CMakePushCheckState)
include(CheckIncludeFileCXX)

check_include_file_cxx(asio.hpp HAS_ASIO_HPP "-pthread -DASIO_STANDALONE -std=c++11")
if(HAS_ASIO_HPP)
    add_definitions("-DHAS_ASIO_HPP -DASIO_STANDALONE")
else()
    message(FATAL_ERROR "Need to have asio installed")
endif()

cmake_push_check_state()
set (CMAKE_REQUIRED_INCLUDES "${CMAKE_SOURCE_DIR}/externals/popl/include")
check_include_file_cxx(popl.hpp HAS_POPL_HPP "-std=c++11")
cmake_pop_check_state()
if(HAS_POPL_HPP)
    add_definitions("-DHAS_POPL_HPP")
else()
    message(FATAL_ERROR "Need to have popl installed")
endif()

pkg_search_module(AVAHI REQUIRED avahi-client)
if (AVAHI_FOUND)
    add_definitions("-DHAS_AVAHI")
else()
    message(FATAL_ERROR "Need to have avahi-client installed")
endif (AVAHI_FOUND)

cmake_push_check_state()
set (CMAKE_REQUIRED_INCLUDES "${CMAKE_SOURCE_DIR}/externals/jsonrpcpp/lib")
check_include_file_cxx(json.hpp JSONRPCPP_FOUND "-std=c++11")
cmake_pop_check_state()
if (JSONRPCPP_FOUND)
    add_definitions("-DHAS_JSONRPCPP")
else()
    message(FATAL_ERROR "Need to have jsonrpcpp installed")
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/externals/jsonrpcpp)
add_subdirectory(src)