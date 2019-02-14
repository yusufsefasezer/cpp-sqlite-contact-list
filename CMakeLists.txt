cmake_minimum_required(VERSION 2.8)

if (CMAKE_VERSION VERSION_LESS "3.1")
  add_definitions(-std=c++11)
else()
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

project(CppContactList)

include_directories(include)
include_directories(lib/sqlite3)

add_subdirectory(lib/sqlite3)

set(SOURCES
	main.cpp
	src/App.cpp
	src/User.cpp
	include/App.h
	include/User.h
	include/Header.h)

add_executable(CppContactList ${SOURCES})

target_link_libraries(CppContactList sqlite3)

if (UNIX)
	target_link_libraries(CppContactList pthread)
	if (NOT APPLE)
		target_link_libraries(CppContactList dl)
	endif ()
elseif (MSYS OR MINGW)
	target_link_libraries(CppContactList ssp)
endif ()

install(TARGETS CppContactList DESTINATION bin)