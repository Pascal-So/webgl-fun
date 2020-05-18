cmake_minimum_required(VERSION 3.5)

project(triangle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")


include(FetchContent)

# Fetch glad and run it. This requires python3.
# This step generates `glad.c` and `glad.h`, which are
# then available as the cmake target `glad`.
# The generated glad configuration is for OpenGL 3.3 without extensions.
set(GLAD_API "gl=3.3" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
FetchContent_Declare(
	glad
	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
	GIT_TAG        v0.1.33
)
FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
	FetchContent_Populate(glad)
	add_subdirectory("${glad_SOURCE_DIR}" "${glad_BINARY_DIR}")
endif()


add_executable(triangle "src/main.cpp")

if (EMSCRIPTEN)
	message(STATUS "Building for emscripten")

	set_target_properties(triangle PROPERTIES OUTPUT_NAME "index")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s USE_WEBGL2=1 -s USE_GLFW=3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -s ASSERTIONS=1 -s SAFE_HEAP=1" )
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -flto")

	target_link_libraries(triangle
		glad
	)

	configure_file("${CMAKE_CURRENT_LIST_DIR}/index.html" "${CMAKE_CURRENT_BINARY_DIR}/index.html" COPYONLY)
else()
	message(STATUS "Building natively")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -ggdb -fno-omit-frame-pointer -D_GLIBCXX_DEBUG -fsanitize=address -fsanitize=leak -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=undefined" )
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -DNDEBUG -flto")

	find_package(glfw3 REQUIRED)

	target_link_libraries(triangle
		glfw
		glad
		${CMAKE_DL_LIBS}
	)
endif()
