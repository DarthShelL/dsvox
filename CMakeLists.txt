cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 11)
set(PROJECT_NAME dsvox)

project(${PROJECT_NAME})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(glad/include)

add_executable(${PROJECT_NAME}
        main.cpp
        window/Window.cpp
        window/Window.h
        window/Events.cpp
        window/Events.h
        graphics/Mesh.cpp
        graphics/Mesh.h
        graphics/Shader.cpp
        graphics/Shader.h)

add_subdirectory(glad/)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(glfw-3.3.6)
target_link_libraries(${PROJECT_NAME} glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

if (APPLE)
    set(RESOURCE_FILES
        resources/shaders/simple.vsh
        resources/shaders/simple.fsh)

    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget
        RESOURCE "${RESOURCE_FILES}")
endif()