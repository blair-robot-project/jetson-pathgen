cmake_minimum_required(VERSION 2.8)
project(jetson_pathgen)

if (CMAKE_VERSION VERSION_LESS "3.1")
	if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set (CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
	endif ()
else ()
	set (CMAKE_CXX_STANDARD 11)
endif ()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(PROJECT_SOURCE_DIR src)

set(PROJECT_BINARY_DIR include)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_BINARY_DIR} gen pathgen/include)

set(SOURCE_FILES pathgen/include/pathfinder.h gen/pathRequestFile.pb.cc gen/pathFile.pb.cc ${PROJECT_SOURCE_DIR}/main.cpp)

add_executable(jetson_pathgen ${SOURCE_FILES})

target_link_libraries(jetson_pathgen ${CONAN_LIBS})
target_link_libraries(jetson_pathgen "/media/LinHDD/Documents/CLion Projects/jetson-pathgen/pathgen/lib/libpathfinder.a")