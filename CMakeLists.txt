cmake_minimum_required(VERSION 3.24)
project(ia_devs_cd_poc
        VERSION 0.1.0
        DESCRIPTION "A proof of concept of a highly scalable IA-DEVS simulator"
        HOMEPAGE_URL https://github.com/sdavtaker/ia-devs-cd-poc
        )

include(CTest)

# Project wide setup
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setting build to hide symbols in targets by default
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

# Preventing writes to package registry by default
set(CMAKE_EXPORT_NO_PACKAGE_REGISTRY YES)

option(ENABLE_COVERAGE "Enable coverage instrumentation" OFF)
if (ENABLE_COVERAGE)
    message(STATUS "Building with coverage instrumentation")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
endif ()
# Validating config type and setting default if needed
get_property(is_multi_conf_build GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (NOT is_multi_conf_build)
    set(allowed_build_types Debug Release RelWithDebInfo MinSizeRel)
    # cmake-gui helper
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${allowed_build_types}")
    if (NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Debug' as none was specified.")
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
    elseif (NOT CMAKE_BUILD_TYPE IN_LIST allowed_build_types)
        message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
    endif ()
endif ()

# Dependencies
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(dependencies)

# Generating code
message(STATUS "Generating header file for library version introspections: ${CMAKE_PROJECT_VERSION} ")
#configure_file(
#        version.h.in
#        include/cadmium/version.h
#        @ONLY
#)

message(STATUS "Generating source file for library version introspections: ${CMAKE_PROJECT_VERSION} ")
#configure_file(
#        version.cpp.in
#        src/version.cpp
#        @ONLY
#)

# Code to build
add_subdirectory(include)
add_subdirectory(src)

# -- Tests and packaging if running this as top project --
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(test)
    add_subdirectory(packaging)
endif ()
