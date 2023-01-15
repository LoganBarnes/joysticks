// ///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Logan Barnes - All Rights Reserved
// ///////////////////////////////////////////////////////////////////////////////////////
#pragma once

// project
#include "ltb/utils/expected.hpp"

// external
#include <tl/expected.hpp>

// standard
#include <memory>

struct GLFWwindow;
struct ImGuiContext;

namespace ltb::joy
{

class MainWindow
{
public:
    explicit MainWindow( );

    auto run( ) -> utils::Expected< MainWindow* >;

private:
    /// \brief RAII object to handle a GLFW context.
    std::shared_ptr< int > glfw_ = nullptr;

    /// \brief RAII object to handle a GLFW window.
    std::shared_ptr< GLFWwindow > window_ = nullptr;

    /// \brief RAII object to handle an ImGui context.
    std::shared_ptr< ImGuiContext > imgui_ = nullptr;

    auto init_glfw( ) -> utils::Expected< MainWindow* >;
    auto init_window( ) -> utils::Expected< MainWindow* >;
    auto init_opengl( ) -> utils::Expected< MainWindow* >;
    auto init_imgui( ) -> utils::Expected< MainWindow* >;
    auto main_loop( ) -> utils::Expected< MainWindow* >;

    [[nodiscard]] auto window( ) const -> GLFWwindow*;
};

} // namespace ltb::joy
