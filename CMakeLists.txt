cmake_minimum_required(VERSION 3.13.2)

set(PROJ_NAME puma)
project(${PROJ_NAME} CXX C)
set(EXE_NAME ${PROJ_NAME})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SDL_SHARED_DEFAULT OFF)
set(SDL_STATIC_DEFAULT ON)
add_subdirectory(vendor/SDL)

include_directories(src)

add_executable(${PROJ_NAME}
	src/main.cpp
)

target_link_libraries(${PROJ_NAME} stdc++exp SDL3-static)

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
