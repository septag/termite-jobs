cmake_minimum_required(VERSION 3.3)
project(termite-jobs)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install CACHE PATH "Install path" FORCE)

# Default output directories for runtime and libraries
if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)
endif()
if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)
endif()

if (NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

if (CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;" CACHE STRING "" FORCE)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Utility)

# Check dependencies
# Compilation flags
if(MSVC)
    add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
    add_definitions(-D_HAS_EXCEPTIONS=0)
    add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
    add_definitions(-D_SCL_SECURE=0)
    add_definitions(-D_SECURE_SCL=0)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    remove_cxx_flag("/EHsc")
    remove_cxx_flag("/GR")
    add_compile_options(/GR-)
else()
    add_compile_options(-std=c++11 -fPIC -fno-rtti -fno-exceptions -Wno-switch)
endif()

# http://stackoverflow.com/questions/33829152/for-cmake-can-you-modify-the-release-debug-compiler-flags-with-add-compiler-fl
set(DEBUG_COMPILE_OPTIONS "-DBX_CONFIG_ALLOCATOR_DEBUG")
add_compile_options("$<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>")

# Sub projects
# dependencies
add_subdirectory(deps/deboost.context)
add_subdirectory(deps/bx)
set_target_properties(fcontext PROPERTIES FOLDER Deps)

add_library(termite-jobs STATIC 
    "termite_jobs.cpp"
    "termite_jobs.h")

target_link_libraries(termite-jobs PRIVATE fcontext bx)
target_include_directories(termite-jobs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
