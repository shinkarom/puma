cmake_minimum_required(VERSION 3.13.2)

set(PROJ_NAME puma)
project(${PROJ_NAME} CXX C)
set(EXE_NAME ${PROJ_NAME})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SDL_SHARED_DEFAULT OFF)
set(SDL_STATIC_DEFAULT ON)
add_subdirectory(vendor/SDL)

include_directories(
	src
	vendor/imgui
	vendor/imgui/backends/
)

add_library(musashi STATIC 
	src/musashi/softfloat/softfloat.c
	src/musashi/m68kcpu.c
	src/musashi/m68kdasm.c
	src/musashi/m68kfpu.c
	src/musashi/m68kops.c
)
target_include_directories(musashi PUBLIC src/musashi)

add_executable(${PROJ_NAME}
	vendor/imgui/imgui.cpp
	vendor/imgui/imgui_demo.cpp
	vendor/imgui/imgui_draw.cpp
	vendor/imgui/imgui_tables.cpp
	vendor/imgui/imgui_widgets.cpp
	vendor/imgui/backends/imgui_impl_sdl3.cpp
	vendor/imgui/backends/imgui_impl_opengl3.cpp
	vendor/imgui/misc/cpp/imgui_stdlib.cpp
	src/ImGuiFileDialog.cpp
	src/gl.c
	src/tsf.c
	src/pathfind.cpp
	src/main.cpp
	src/render.cpp
	src/apu.cpp
	src/cpu.cpp
	src/bus.cpp
	src/ppu.cpp
	src/input.cpp
	src/color.cpp
)

target_link_libraries(${PROJ_NAME} stdc++exp SDL3-static musashi)

set_target_properties(${PROJ_NAME} PROPERTIES
	LINKER_LANGUAGE CXX
)

target_compile_options(${EXE_NAME} PUBLIC 
	-g -Wall 
	$<$<CONFIG:Debug>:-g> 
	$<$<CONFIG:Release>: -O2>
)
target_link_options(${EXE_NAME} PUBLIC 
	$<$<CONFIG:RELEASE>: -s>
)
