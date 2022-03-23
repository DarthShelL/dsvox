cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 11)
set(PROJECT_NAME dsvox)

project(${PROJECT_NAME})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(glad/include)
include_directories(sources)

add_executable(${PROJECT_NAME}
        main.cpp
        sources/window/Window.cpp
        sources/window/Window.h
        sources/window/Events.cpp
        sources/window/Events.h
        sources/graphics/Mesh.cpp
        sources/graphics/Mesh.h
        sources/graphics/Shader.cpp
        sources/graphics/Shader.h)

add_subdirectory(glad/)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(glfw-3.3.6)
target_link_libraries(${PROJECT_NAME} glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# Get list of shader files
set(SUB_DIR "resources/shaders")
FILE(GLOB_RECURSE RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${SUB_DIR}/*")
foreach(RES IN LISTS RESOURCE_FILES)
    get_filename_component(F_NAME ${RES} NAME)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${RES}" "${CMAKE_BINARY_DIR}/${SUB_DIR}/${F_NAME}"
        COMMENT "Copying ${F_NAME} to build directory"
    )
endforeach()
