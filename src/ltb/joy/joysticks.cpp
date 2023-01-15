// ///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Logan Barnes - All Rights Reserved
// ///////////////////////////////////////////////////////////////////////////////////////
#include "ltb/joy/joysticks.hpp"

// external
#include <GLFW/glfw3.h>
#include <imgui.h>

// standard
#include <numeric>

namespace ltb::joy
{

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

auto configure_gui( std::vector< Joystick > const& joysticks ) -> void
{
    if ( joysticks.empty( ) )
    {
        ImGui::TextColored( { 1.f, 1.f, 0.f, 1.f }, "No joysticks detected" );
        return;
    }

    for ( auto& joystick : joysticks )
    {
        ImGui::PushID( joystick.glfw_index );

        if ( ImGui::CollapsingHeader( joystick.name.c_str( ) ) )
        {
            auto const button_column_count = std::min( 8UL, joystick.buttons.size( ) );

            if ( ImGui::BeginTable( "Buttons", button_column_count, ImGuiTableFlags_Borders ) )
            {

                ImGui::TableNextRow( );

                for ( auto i = 0UL; i < joystick.buttons.size( ); ++i )
                {
                    ImGui::PushID( static_cast< int >( i ) );

                    ImGui::TableNextColumn( );

                    auto const label = fmt::format( "({})##button", i );
                    ImGui::RadioButton( label.c_str( ), joystick.buttons[ i ] == GLFW_PRESS );

                    if ( i % 8 == 7 )
                    {
                        ImGui::TableNextRow( );
                    }

                    ImGui::PopID( );
                }

                ImGui::EndTable( );
            }

            for ( auto i = 0UL; i < joystick.axes.size( ); ++i )
            {
                ImGui::PushID( static_cast< int >( i ) );

                auto const label = fmt::format( "({})##axis", i );
                auto       axis  = joystick.axes[ i ];
                ImGui::SliderFloat( label.c_str( ), &axis, -1.f, 1.f, "%.3f" );

                ImGui::PopID( );
            }
        }

        ImGui::PopID( );
    }
}

} // namespace ltb::joy
