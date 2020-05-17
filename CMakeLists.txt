cmake_minimum_required(VERSION 3.7)

project(triangle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(triangle "src/main.cpp")

if (EMSCRIPTEN)
    message(STATUS "Building for emscripten")

    set_target_properties(triangle PROPERTIES OUTPUT_NAME "index")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_WEBGL2=1")

    configure_file("${CMAKE_CURRENT_LIST_DIR}/index.html" "${CMAKE_CURRENT_BINARY_DIR}/index.html" COPYONLY)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fno-rtti")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -ggdb -fno-omit-frame-pointer -D_GLIBCXX_DEBUG -fsanitize=address -fsanitize=leak -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=undefined" )
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -DNDEBUG -flto")

	list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
	find_package(OpenGL REQUIRED)
    find_package(SDL2 REQUIRED)
    find_package(GLEW REQUIRED)

	target_link_libraries(triangle
		SDL2::Main
		OpenGL::GL
		GLEW::GLEW
	)
endif()
