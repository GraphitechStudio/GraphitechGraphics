cmake_minimum_required(VERSION 3.15)
# Dispatch graphics backend subdirectories
if(GRAPHICS_BACKEND STREQUAL "OpenGL")
    add_subdirectory(OpenGL)
endif()
