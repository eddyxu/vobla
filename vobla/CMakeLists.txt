add_library (vobla
	command.cpp
	sysinfo.cpp
	status.cpp
	)

include_directories(${gtest_SOURCE_DIR}/include)
include(../cmake_modules/util.cmake)

set(TEST_LIBS vobla gtest gmock_main)

file(GLOB TEST_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/*_test.cpp")

foreach(testFile ${TEST_FILES})
	string(REGEX REPLACE ".cpp\$" "" testName "${testFile}")
	cxx_test("${testName}" "${TEST_LIBS}")
endforeach(testFile)

add_subdirectory(gutil)
