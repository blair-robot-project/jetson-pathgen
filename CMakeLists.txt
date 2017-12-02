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

set(SOURCE_FILES pathgen/src/modifiers/swerve.c pathgen/src/modifiers/tank.c pathgen/src/followers/distance.c
		pathgen/src/followers/encoder.c pathgen/src/fit.c
		pathgen/src/io.c pathgen/src/mathutil.c pathgen/src/spline.c pathgen/src/trajectory.c
		pathgen/src/generator.c pathgen/include/io.h pathgen/include/mathutil.h pathgen/include/spline.h pathgen/include/trajectory.h
		pathgen/include/generator.h pathgen/include/lib.h pathgen/include/fit.h pathgen/include/structs.h
		pathgen/include/modifiers/tank.h pathgen/include/modifiers/swerve.h pathgen/include/followers/distance.h
		pathgen/include/followers/encoder.h
		gen/pathRequestFile.pb.cc gen/pathFile.pb.cc ${PROJECT_BINARY_DIR}/main.h
		${PROJECT_SOURCE_DIR}/main.cpp)

add_executable(jetson_pathgen ${SOURCE_FILES})

target_link_libraries(jetson_pathgen ${CONAN_LIBS})