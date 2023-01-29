// ///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Logan Barnes - All Rights Reserved
// ///////////////////////////////////////////////////////////////////////////////////////
#include "ltb/joy/joysticks.hpp"

// external
#include <GLFW/glfw3.h>
#include <imgui.h>

// standard
#include <algorithm>

namespace ltb::joy
{
namespace
{

auto configure_buttons_gui( Joystick const& joystick )
{
    using size_type = std::decay_t< decltype( joystick.buttons.size( ) ) >;

    auto constexpr max_column_count = size_type( 8 );
    auto const button_column_count  = std::min( max_column_count, joystick.buttons.size( ) );

    if ( ImGui::BeginTable( "Buttons", button_column_count, ImGuiTableFlags_Borders ) )
    {
        ImGui::TableNextRow( );

        for ( auto i = 0ULL; i < joystick.buttons.size( ); ++i )
        {
            ImGui::PushID( static_cast< int >( i ) );

            ImGui::TableNextColumn( );

            auto const label = fmt::format( "({})##button", i );
            ImGui::RadioButton( label.c_str( ), joystick.buttons[ i ] == GLFW_PRESS );

            if ( i % max_column_count == max_column_count - 1ULL )
            {
                ImGui::TableNextRow( );
            }

            ImGui::PopID( );
        }

        ImGui::EndTable( );
    }
}

auto configure_axis_gui( Joystick const& joystick )
{
    for ( auto i = 0UL; i < joystick.axes.size( ); ++i )
    {
        ImGui::PushID( static_cast< int >( i ) );

        auto const label = fmt::format( "({})##axis", i );
        auto       axis  = joystick.axes[ i ];
        ImGui::SliderFloat( label.c_str( ), &axis, -1.f, 1.f, "%.3f" );

        ImGui::PopID( );
    }
}

} // namespace

auto poll_joystick_info( ) -> std::vector< Joystick >
{
    auto joysticks = std::vector< Joystick >{ };

    for ( int glfw_joystick_index = 0; glfw_joystick_index <= GLFW_JOYSTICK_LAST; ++glfw_joystick_index )
    {
        if ( glfwJoystickPresent( glfw_joystick_index ) == GLFW_TRUE )
        {
            auto joystick       = Joystick{ };
            joystick.name       = glfwGetJoystickName( glfw_joystick_index );
            joystick.glfw_index = glfw_joystick_index;

            // Copy all the pointer data so there is no concern about references disappearing.
            auto        count = 0;
            auto const* axes  = glfwGetJoystickAxes( glfw_joystick_index, &count );
            joystick.axes     = { axes, axes + count };

            auto const* buttons = glfwGetJoystickButtons( glfw_joystick_index, &count );
            joystick.buttons    = { buttons, buttons + count };

            joysticks.emplace_back( joystick );
        }
    }

    return joysticks;
}

auto configure_gui_window( std::vector< Joystick > const& joysticks ) -> void
{
    ImGui::SetNextWindowPos( { 0.f, 0.f } );
    ImGui::SetNextWindowSize( ImGui::GetIO( ).DisplaySize );
    if ( ImGui::Begin( "Joysticks", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize ) )
    {
        if ( joysticks.empty( ) )
        {
            ImGui::TextColored( { 1.f, 1.f, 0.f, 1.f }, "No joysticks detected" );
        }
        else
        {
            for ( auto& joystick : joysticks )
            {
                ImGui::PushID( joystick.glfw_index );

                if ( ImGui::CollapsingHeader( joystick.name.c_str( ), ImGuiTreeNodeFlags_DefaultOpen ) )
                {
                    configure_buttons_gui( joystick );
                    configure_axis_gui( joystick );
                }

                ImGui::PopID( );
            }
        }
    }
    ImGui::End( );
}

} // namespace ltb::joy
