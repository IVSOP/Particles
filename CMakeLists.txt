cmake_minimum_required(VERSION 3.22.1) # 3.13.4

project(GAME)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -pedantic -Wno-unused-parameter -mavx2") # -O2??? -ftree-vectorize -fopt-info-vec-missed or all or optimized
# SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
# SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/GLErrors.cpp
	src/Renderer.cpp
	src/Sandbox.cpp
	src/Grid.cpp
	src/Simulator.cpp
	src/common.cpp
	src/Spawner.cpp

	external/imgui/src/imgui.cpp
	external/imgui/src/imgui_draw.cpp
	external/imgui/src/imgui_widgets.cpp
	external/imgui/src/imgui_demo.cpp
	external/imgui/src/imgui_tables.cpp
	external/imgui/src/imgui_impl_glfw.cpp
	external/imgui/src/imgui_impl_opengl3.cpp
)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
	include
	external/imgui/include
	external/stb
)

# wtf??
add_subdirectory(
	external/glfw
	src
) # build

add_subdirectory(external/glm)

# unecessary since it was specified above?
target_include_directories(${PROJECT_NAME} # include
	PUBLIC include
	PUBLIC external/glfw/include
	PUBLIC external/imgui/include
	PUBLIC external/stb
	PUBLIC external/glm
)

target_link_directories(${PROJECT_NAME} #link
	PRIVATE external/glfw/src
	# PRIVATE src
)

target_link_libraries(${PROJECT_NAME}
	glfw
	GLEW::GLEW
	OpenGL::GL
	glm::glm
)
