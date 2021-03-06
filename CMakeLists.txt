cmake_minimum_required(VERSION 2.8)
project(vobla)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake_modules)
include(cmake_modules/util.cmake)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wunused-value -std=c++11 -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-fno-strict-aliasing -O3 -DNDEBUG")

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	execute_process(
		COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
		message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
	endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
	message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()

include(ExternalProject)

if (VOBLA_TEST)
	# Gmock
	ExternalProject_Add(googlemock
		URL "https://googlemock.googlecode.com/files/gmock-1.7.0.zip"
		SOURCE_DIR "${CMAKE_BINARY_DIR}/third_party/gmock"
		CMAKE_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}" "-DCMAKE_CXX_FLAGS='${CMAKE_CXX_FLAGS}'"
		INSTALL_COMMAND "")
	ExternalProject_Get_Property(googlemock source_dir)
	include_directories(${source_dir}/include ${source_dir}/gtest/include)
	ExternalProject_Get_Property(googlemock binary_dir)
	link_directories(${binary_dir} ${binary_dir}/gtest)
	enable_testing()
endif()

find_package(Boost REQUIRED)
find_package(Glog REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(SYSTEM ${Boost_INCLUDE_DIR} ${GLOG_INCLUDE_DIR})

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(vobla)
