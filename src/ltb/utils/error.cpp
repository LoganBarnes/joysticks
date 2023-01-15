// ///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Logan Barnes - All Rights Reserved
// ///////////////////////////////////////////////////////////////////////////////////////
#include "ltb/utils/error.hpp"

namespace ltb::utils
{

SourceLocation::SourceLocation( std::filesystem::path file, int line )
    : filename( std::move( file ) )
    , line_number( line )
{
}

Error::Error( SourceLocation source_location, Severity severity, std::string error_message )
    : source_location_( std::move( source_location ) )
    , severity_( severity )
    , error_message_( std::move( error_message ) )
    , debug_message_(
          fmt::format( "[{}:{}] {}", source_location_.filename.string( ), source_location_.line_number, error_message_ )
      )
{
}

auto Error::severity( ) const -> Error::Severity const&
{
    return severity_;
}

auto Error::error_message( ) const -> std::string const&
{
    return error_message_;
}

auto Error::debug_error_message( ) const -> std::string const&
{
    return debug_message_;
}

auto Error::source_location( ) const -> SourceLocation const&
{
    return source_location_;
}

auto Error::operator==( Error const& other ) const -> bool
{
    return debug_message_ == other.debug_message_;
}

auto Error::operator!=( Error const& other ) const -> bool
{
    return !( this->operator==( other ) );
}

} // namespace ltb::utils
