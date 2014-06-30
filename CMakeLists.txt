cmake_minimum_required(VERSION 2.8)
project(cmake_cpp11 CXX)

include(cmake_modules/util.cmake)
include(ExternalProject)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wunused-value -std=c++11 -fPIC")

include(CheckCXXCompilerFlag)
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

# Gmock
ExternalProject_Add(googlemock
    URL "https://googlemock.googlecode.com/files/gmock-1.7.0.zip"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/third_party/gmock"
    INSTALL_COMMAND "")

#if (NOT EXISTS "test/gmock-1.7.0/")
#  execute_process(COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/test )
#  execute_process(COMMAND wget https://googlemock.googlecode.com/files/gmock-1.7.0.zip -O ${CMAKE_SOURCE_DIR}/test/gmock.zip)
#  execute_process(COMMAND unzip gmock.zip WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
#endif()

find_package(boost)

enable_testing()
include_directories(.)
add_subdirectory(test/gmock-1.7.0)
add_subdirectory(vobla)
