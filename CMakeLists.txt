cmake_minimum_required(VERSION 3.20)

project(Parkour LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(Parkour
	${SOURCES}
)
target_compile_options(Parkour PRIVATE /EHsc)

target_include_directories(Parkour PRIVATE
	D:/include/glm/
	D:/include/glad/
	D:/include/assimp/
	D:/include/glfw-3.5.1/
	D:/include/nlohmann/
	${CMAKE_SOURCE_DIR}/include/
)

target_link_libraries(Parkour PRIVATE 
	D:/libs/glfw3.lib
	D:/libs/assimp-vc143-mtd.lib
   opengl32
)
