// ///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Logan Barnes - All Rights Reserved
// ///////////////////////////////////////////////////////////////////////////////////////
#pragma once

// external
#include <spdlog/fmt/fmt.h>

// standard
#include <filesystem>
#include <string>

///\brief Macro used to auto-fill line and file information when creating an error.
/// \code
/// ltb::utils::Error error = LTB_MAKE_ERROR("Bad thing happened!");
/// assert(error.severity() == ltb::utils::Error::Severity::Error);
/// \endcode
// #define LTB_MAKE_ERROR(msg) ::ltb::utils::Error({__FILE__, __LINE__}, ::ltb::utils::Error::Severity::Error, msg)
#define LTB_MAKE_ERROR( ... )                                                                                          \
    ::ltb::utils::Error( { __FILE__, __LINE__ }, ::ltb::utils::Error::Severity::Error, fmt::format( __VA_ARGS__ ) )

///\brief Macro used to auto-fill line and file information when creating a warning.
/// \code
/// ltb::utils::Error error = ROTOR_MAKE_WARNING("Not so bad thing happened!");
/// assert(error.severity() == ltb::utils::Error::Severity::Warning);
/// \endcode
// #define LTB_MAKE_WARNING(msg) ::ltb::utils::Error({__FILE__, __LINE__}, ::ltb::utils::Error::Severity::Warning, msg)
#define LTB_MAKE_WARNING( ... )                                                                                        \
    ::ltb::utils::Error( { __FILE__, __LINE__ }, ::ltb::utils::Error::Severity::Warning, , fmt::format( __VA_ARGS__ ) )

namespace ltb::utils
{

struct SourceLocation
{
    std::filesystem::path filename;
    int                   line_number;

    SourceLocation( ) = delete;
    SourceLocation( std::filesystem::path file, int line );
};

///\brief A simple class used to pass error messages around.
class Error
{
public:
    enum class Severity
    {
        Error,
        Warning,
    };

    Error( ) = delete;
    explicit Error( SourceLocation source_location, Severity severity, std::string error_message );

    [[nodiscard]] auto severity( ) const -> Severity const&;
    [[nodiscard]] auto error_message( ) const -> std::string const&;
    [[nodiscard]] auto debug_error_message( ) const -> std::string const&;
    [[nodiscard]] auto source_location( ) const -> SourceLocation const&;

    auto operator==( Error const& other ) const -> bool;
    auto operator!=( Error const& other ) const -> bool;

private:
    SourceLocation source_location_; ///< File and line number where error was created
    Severity       severity_; ///< The type of error (warning or error)
    std::string    error_message_; ///< The error message
    std::string    debug_message_; ///< Error message with file and line number "[file:line] error message"
};

} // namespace ltb::utils
