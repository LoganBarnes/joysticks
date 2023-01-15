// ///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Logan Barnes - All Rights Reserved
// ///////////////////////////////////////////////////////////////////////////////////////
#include "ltb/joy/app.hpp"

// external
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

namespace ltb::joy
{
namespace
{

constexpr auto window_title  = "Joysticks";
constexpr auto window_width  = 800;
constexpr auto window_height = 600;

} // namespace

MainWindow::MainWindow( ) = default;

auto MainWindow::run( ) -> utils::Expected< MainWindow* >
{
    return init_glfw( )
        .and_then( &MainWindow::init_window )
        .and_then( &MainWindow::init_opengl )
        .and_then( &MainWindow::init_imgui )
        .and_then( &MainWindow::main_loop );
}

auto MainWindow::init_glfw( ) -> utils::Expected< MainWindow* >
{
    // Set the error callback before any GLFW calls.
    glfwSetErrorCallback( []( int /*error*/, char const* description ) {
        spdlog::error( "GLFW ERROR: {}", description );
    } );

    glfw_ = std::shared_ptr< int >( new int( glfwInit( ) ), []( auto* p ) {
        spdlog::debug( "Terminating GLFW" );
        glfwTerminate( ); // safe even if glfwInit() fails
        delete p;
    } );

    if ( *glfw_ == 0 )
    {
        return LTB_MAKE_UNEXPECTED_ERROR( "glfwInit() failed" );
    }
    spdlog::debug( "GLFW Initialized" );

    return this;
}

auto MainWindow::init_window( ) -> utils::Expected< MainWindow* >
{

    glfwWindowHint( GLFW_SAMPLES, 4 );
    glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE );
    glfwWindowHint( GLFW_VISIBLE, GLFW_TRUE );

    // Make sure we have a decent OpenGL Profile.
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 ); // This is the highest macOS will support.
    glfwWindowHint( GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API );

    window_ = std::shared_ptr< GLFWwindow >(
        glfwCreateWindow( window_width, window_height, window_title, nullptr, nullptr ), //
        []( auto* p ) {
            if ( p )
            {
                spdlog::debug( "Destroying GLFW window" );
                glfwDestroyWindow( p );
            }
        }
    );

    if ( window_ == nullptr )
    {
        return LTB_MAKE_UNEXPECTED_ERROR( "GLFW window creation failed" );
    }
    spdlog::debug( "GLFW window created" );

    glfwMakeContextCurrent( window( ) );

    // Enable vsync
    glfwSwapInterval( 1 );

    return this;
}

auto MainWindow::init_opengl( ) -> utils::Expected< MainWindow* >
{
    if ( gl3wInit( ) )
    {
        return LTB_MAKE_UNEXPECTED_ERROR( "Failed to load OpenGL via gl3wInit()" );
    }
    spdlog::debug( "OpenGL loaded" );

    auto const* vendor   = glGetString( GL_VENDOR );
    auto const* renderer = glGetString( GL_RENDERER );

    spdlog::info( "OpenGL Device: {} {}", vendor, renderer );

    return this;
}

auto MainWindow::init_imgui( ) -> utils::Expected< MainWindow* >
{

    return this;
}

auto MainWindow::main_loop( ) -> utils::Expected< MainWindow* >
{
    while ( !glfwWindowShouldClose( window( ) ) )
    {
        auto framebuffer_width  = 0;
        auto framebuffer_height = 0;
        glfwGetFramebufferSize( window( ), &framebuffer_width, &framebuffer_height );
        glViewport( 0, 0, framebuffer_width, framebuffer_height );

        // Update GUI state
        // ImGui_ImplOpenGL3_NewFrame( );
        // ImGui_ImplGlfw_NewFrame( );
        // ImGui::NewFrame( );

        // Gather all available joystick info

        // Configure joysticks GUI

        // Render GUI
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        // ImGui::Render( );
        // ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );

        glfwSwapBuffers( window( ) );
        glfwPollEvents( );
    }

    return this;
}

auto MainWindow::window( ) const -> GLFWwindow*
{
    return window_.get( );
}

} // namespace ltb::joy
