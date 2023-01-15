# ##############################################################################
# Copyright (c) 2022 Logan Barnes - All Rights Reserved
# ##############################################################################

### System Packages ###
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

# Parallel execution policies work out of the box with MSVC.
# For GCC and Clang (and maybe Intel?) we need to find and
# link Intel's Thread Building Blocks (TBB) library.
if (NOT MSVC)
  # MacOS: brew install tbb
  # Linux: sudo apt update libtbb-dev
  find_package(TBB REQUIRED)
endif ()

### External Repositories ###
cpmaddpackage(
  NAME
    expected
  GIT_TAG
    96d547c03d2feab8db64c53c3744a9b4a7c8f2c5 # master
  GITHUB_REPOSITORY
    TartanLlama/expected
  OPTIONS
    "EXPECTED_BUILD_TESTS OFF"
)
cpmaddpackage(
  "gh:Neargye/magic_enum@0.7.3"
)
cpmaddpackage(
  "gh:gabime/spdlog@1.9.2"
)
cpmaddpackage(
  NAME
    range-v3
  GITHUB_REPOSITORY
    ericniebler/range-v3
  GIT_TAG
    0.11.0
  OPTIONS
    "RANGES_CXX_STD 17"
  DOWNLOAD_ONLY TRUE
)

# GUI
cpmaddpackage(
  NAME
    GLFW
  GIT_REPOSITORY
    https://github.com/glfw/glfw.git
  GIT_TAG
    3.3.8
  OPTIONS
    "GLFW_BUILD_TESTS OFF"
    "GLFW_BUILD_EXAMPLES OFF"
    "GLFW_BULID_DOCS OFF"
)
cpmaddpackage(
  NAME
    imgui
  GITHUB_REPOSITORY
    ocornut/imgui
  GIT_TAG
    ad5aa5416630059c9213a4cfe00a827c3991e964 # docking branch v1.88
  DOWNLOAD_ONLY
    TRUE
)

if (magic_enum_ADDED)
  # Mark external include directories as system includes to avoid warnings.
  target_include_directories(
    magic_enum
    SYSTEM
    INTERFACE
      $<BUILD_INTERFACE:${magic_enum_SOURCE_DIR}/include>
  )
endif ()

if (spdlog_ADDED)
  # Mark external include directories as system includes to avoid warnings.
  target_include_directories(
    spdlog
    SYSTEM
    INTERFACE
      $<BUILD_INTERFACE:${spdlog_SOURCE_DIR}/include>
  )
endif ()

if (range-v3_ADDED)
  add_library(
    range-v3
    INTERFACE
  )
  add_library(
    range-v3::range-v3
    ALIAS
      range-v3
  )

  # Mark external include directories as system includes to avoid warnings.
  target_include_directories(
    range-v3
    SYSTEM
    INTERFACE
      $<BUILD_INTERFACE:${range-v3_SOURCE_DIR}/include>
  )
  # Fix build warnings on Windows
  target_compile_options(
    range-v3
    INTERFACE
      $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->
      $<$<COMPILE_LANG_AND_ID:CUDA,MSVC>:-Xcompiler=/permissive->
  )
endif ()

if (GLFW_ADDED)
  add_library(
    glfw::glfw
    ALIAS
      glfw
  )
endif ()

if (imgui_ADDED)
  add_library(
    imgui
    ${imgui_SOURCE_DIR}/imconfig.h
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imstb_rectpack.h
    ${imgui_SOURCE_DIR}/imstb_textedit.h
    ${imgui_SOURCE_DIR}/imstb_truetype.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  )
  add_library(
    imgui::imgui
    ALIAS
      imgui
  )
  target_link_libraries(
    imgui
    PUBLIC
      OpenGL::GL
      glfw::glfw
  )
  # Mark external include directories as system includes to avoid warnings.
  target_include_directories(
    imgui
    SYSTEM
    PUBLIC
      $<BUILD_INTERFACE:${imgui_SOURCE_DIR}>
      $<BUILD_INTERFACE:${imgui_SOURCE_DIR}/backends>
  )
  target_compile_features(
    imgui
    PUBLIC
      cxx_std_11
  )
  target_compile_options(
    imgui
    PRIVATE # Ignore build warning on this third-party code
      $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-w>
  )
endif ()
