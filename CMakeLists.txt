# ##############################################################################
# Copyright (c) 2023 Logan Barnes - All Rights Reserved
# ##############################################################################
cmake_minimum_required(VERSION 3.24)
project(Joysticks)

# Prevent cyclic dependencies even for static libs
set(GLOBAL_DEPENDS_NO_CYCLES ON)

# Prevent cmake from creating build artifacts in the source directory
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# ##############################################################################
# Options
# ##############################################################################
option(LTB_JOYSTICKS_USE_STRICT_FLAGS "Use strict flags when building" OFF)

# ##############################################################################
# CMake Package Manager
# ##############################################################################
include(cmake/CPM.cmake)

# ##############################################################################
# External Packages
# ##############################################################################
include(${CMAKE_CURRENT_LIST_DIR}/cmake/ThirdParty.cmake)

# ##############################################################################
# Library
# ##############################################################################
file(
  GLOB_RECURSE
    Joysticks_SOURCE_FILES
  LIST_DIRECTORIES
    false
  CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/*.hpp
)

add_executable(
  joysticks
  ${Joysticks_SOURCE_FILES}
)

target_link_libraries(
  joysticks
  PUBLIC
    # Utils
    Threads::Threads
    tl::expected
    magic_enum::magic_enum
    range-v3::range-v3
    spdlog::spdlog
    $<TARGET_NAME_IF_EXISTS:TBB::tbb>
    # Gui
    imgui::imgui
)
target_include_directories(
  joysticks
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
)
target_compile_features(
  joysticks
  PUBLIC
    cxx_std_20
)
target_compile_definitions(
  joysticks
  PUBLIC
    # Deal with windows warnings and macros.
    $<$<PLATFORM_ID:Windows>:_CRT_SECURE_NO_WARNINGS>
    $<$<PLATFORM_ID:Windows>:NOMINMAX>
)
set_target_properties(
  joysticks
  PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# ##############################################################################
# Development Settings
# ##############################################################################
if (${LTB_JOYSTICKS_USE_STRICT_FLAGS})
  target_compile_options(
    joysticks
    PUBLIC
      # Strict warnings/errors with gcc and clang
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wall>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wextra>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Werror>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wpedantic>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wunused>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-pedantic-errors>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Winit-self>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wold-style-cast>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Woverloaded-virtual>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wsign-conversion>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wshadow>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wmissing-declarations>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wmissing-include-dirs>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wno-unknown-pragmas>
      $<$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang>:-Wimplicit-float-conversion>
      $<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wfloat-conversion>
      # Strict warnings/errors with msvc
      $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/external:W0>
      # Ignore any headers using angle brackets on windows
      $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/experimental:external>
      $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/external:anglebrackets>
      $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/external:W0>
  )
endif ()
