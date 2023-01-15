// ///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Logan Barnes - All Rights Reserved
// ///////////////////////////////////////////////////////////////////////////////////////

// project
#include "ltb/joy/app.hpp"
#include <spdlog/spdlog.h>

using namespace ltb;

auto main( ) -> int
{
    return joy::MainWindow{ }
        .run( )
        .map( []( auto* ) { return EXIT_SUCCESS; } )
        .map_error( []( utils::Error&& error ) {
            spdlog::error( "{}", error.debug_error_message( ) );
            return error;
        } )
        .value_or( EXIT_FAILURE );
}
