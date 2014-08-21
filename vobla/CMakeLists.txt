include_directories(${gtest_SOURCE_DIR}/include)
include(${CMAKE_SOURCE_DIR}/cmake_modules/util.cmake)

add_subdirectory(gutil)

add_library (vobla
	clock.cpp
	command.cpp
	configuration.cpp
	file.cpp
	hash.cpp
	status.cpp
	sysinfo.cpp
	timer.cpp
	)
target_link_libraries(vobla glog crypto)

set(TEST_LIBS vobla gtest gmock_main)

file(GLOB TEST_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/*_test.cpp")

foreach(testFile ${TEST_FILES})
	string(REGEX REPLACE ".cpp\$" "" testName "${testFile}")
	cxx_test("${testName}" "${TEST_LIBS}")
endforeach(testFile)
