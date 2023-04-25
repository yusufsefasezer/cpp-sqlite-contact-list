cmake_minimum_required(VERSION 3.20)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(CppContactList VERSION 0.1
											 DESCRIPTION "A simple contact list application developed with Modern C++ and SQLite."
											 LANGUAGES CXX)

include_directories(include)
include_directories(${SQLite3_INCLUDE_DIRS})
find_package(SQLite3 REQUIRED)

set(SOURCES
	main.cpp
	src/App.cpp
	src/User.cpp
	include/App.h
	include/User.h
	include/Header.h)

add_executable(CppContactList ${SOURCES})

target_link_libraries(CppContactList ${SQLite3_LIBRARIES})

if (UNIX)
	target_link_libraries(CppContactList pthread)
	if (NOT APPLE)
		target_link_libraries(CppContactList dl)
	endif ()
elseif (MSYS OR MINGW)
	target_link_libraries(CppContactList ssp)
endif ()

install(TARGETS CppContactList DESTINATION bin)