// ///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Logan Barnes - All Rights Reserved
// ///////////////////////////////////////////////////////////////////////////////////////
#pragma once

// project
#include "ltb/utils/expected.hpp"

// standard
#include <vector>

namespace ltb::joy
{

struct Joystick
{
    std::string                  name       = "";
    int                          glfw_index = -1;
    std::vector< float >         axes       = { };
    std::vector< unsigned char > buttons    = { };
};

auto poll_joystick_info( ) -> std::vector< Joystick >;

auto configure_gui_window( std::vector< Joystick > const& joysticks ) -> void;

} // namespace ltb::joy
