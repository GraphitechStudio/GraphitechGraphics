cmake_minimum_required(VERSION 3.15)
project(GraphitechGraphicsOpenGL LANGUAGES C CXX)

message(STATUS "Configuring Graphitech OpenGL backend...")

# Collect OpenGL renderer sources from the Graphitech OpenGL backend directory
file(GLOB_RECURSE OPENGL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Render/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(GraphitechGraphicsOpenGL STATIC ${OPENGL_SOURCES})

target_include_directories(GraphitechGraphicsOpenGL PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Render
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Graphitech/Engine
    ${CMAKE_SOURCE_DIR}/Graphitech/ECS
)

# ============ ADD GLM INCLUDES ============
# GLM is provided by CPM
if(TARGET glm)
    target_link_libraries(GraphitechGraphicsOpenGL PUBLIC glm)
    message(STATUS "[OpenGL Backend] Using GLM from CPM")
else()
    message(FATAL_ERROR "[OpenGL Backend] GLM target not found")
endif()

# Link ECS library if available
if(TARGET GraphitechECS)
    target_link_libraries(GraphitechGraphicsOpenGL PUBLIC GraphitechECS)
    message(STATUS "[OpenGL Backend] Linked GraphitechECS")
endif()

# Link common graphics dependencies if available
find_package(PkgConfig QUIET)
if(TARGET glfw)
    target_link_libraries(GraphitechGraphicsOpenGL PUBLIC glfw)
elseif(PkgConfig_FOUND)
    pkg_check_modules(GLFW QUIET REQUIRED glfw3)
    if(GLFW_FOUND)
        target_include_directories(GraphitechGraphicsOpenGL PUBLIC ${GLFW_INCLUDE_DIRS})
        target_link_libraries(GraphitechGraphicsOpenGL PUBLIC ${GLFW_LIBRARIES})
    endif()
endif()

# Link system libs used by the project
target_link_libraries(GraphitechGraphicsOpenGL PUBLIC
    GL
    dl
    pthread
)

# If GLAD was generated in the Engine CMake, reuse it
if(TARGET glad_lib)
    target_link_libraries(GraphitechGraphicsOpenGL PUBLIC glad_lib)
    target_include_directories(GraphitechGraphicsOpenGL PUBLIC "${GLAD_OUT_DIR}/include")
endif()

# Try to locate stb_image.h via a recursive glob in the CPM cache (stb is added by Engine CMake)
file(GLOB_RECURSE STB_HEADERS RELATIVE ${CMAKE_SOURCE_DIR}
    "${CMAKE_SOURCE_DIR}/.cpm-cache/stb/*/stb_image.h"
)
if(STB_HEADERS)
    list(GET STB_HEADERS 0 STB_HEADER_REL)
    set(STB_HEADER_PATH "${CMAKE_SOURCE_DIR}/${STB_HEADER_REL}")
    get_filename_component(STB_DIR ${STB_HEADER_PATH} DIRECTORY)
    target_include_directories(GraphitechGraphicsOpenGL PUBLIC ${STB_DIR})
else()
    message(WARNING "stb_image.h not found in .cpm-cache; texture loading may fail to compile")
endif()

# Compiler flags
target_compile_options(GraphitechGraphicsOpenGL PRIVATE
    -Wall -Wextra
    $<$<CONFIG:Debug>:-g3>
    $<$<CONFIG:Release>:-O3>
)
