function(cxx_test name libs)
	add_executable("${name}" "${name}.cpp")
	target_link_libraries("${name}" "${libs}" -lpthread)
	add_test("${name}" "${name}")
endfunction()
