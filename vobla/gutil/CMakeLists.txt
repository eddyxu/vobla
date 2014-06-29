# Build Google Util

set(CMAKE_CXX_FLAGS "-std=c++11 -funsigned-char -Wno-deprecated -Wno-char-subscripts")

add_library(gutil
	bits.cc
	demangle.cc
	file.cc
	file_util.cc
	int128.cc
	mathlimits.cc
	random.cc
	stringprintf.cc
	strings/ascii_ctype.cc
	strings/case.cc
	strings/charset.cc
	strings/escaping.cc
	strings/human_readable.cc
	strings/join.cc
	strings/memutil.cc
	strings/numbers.cc
	strings/serialize.cc
	strings/split.cc
	strings/strcat.cc
	strings/stringpiece.cc
	strings/strip.cc
	strings/substitute.cc
	strings/util.cc
	strtoint.cc
	walltime.cc
	)
