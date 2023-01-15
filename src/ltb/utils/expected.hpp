// ///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Logan Barnes - All Rights Reserved
// ///////////////////////////////////////////////////////////////////////////////////////
#pragma once

// project
#include "ltb/utils/error.hpp"

namespace tl
{

template < class T, class E >
class [[nodiscard]] expected;

}

// external
#include <tl/expected.hpp>

#define LTB_MAKE_UNEXPECTED_ERROR( ... ) ::tl::make_unexpected( LTB_MAKE_ERROR( __VA_ARGS__ ) )
#define LTB_MAKE_UNEXPECTED_WARNING( ... ) ::tl::make_unexpected( LTB_MAKE_WARNING( __VA_ARGS__ ) )

namespace ltb::utils
{

/// \brief A type of tl::expected that requires the result to be used.
///
/// Example usage:
///
/// \code{.cpp}
///
/// // function that returns an int or 'ltb::utils::Error'
/// auto my_function(int non_negative) -> utils::Expected<int> {
///     if (non_negative < 0) {
///         return tl::make_unexpected(ROTOR_MAKE_ERROR("number is negative"));
///     }
///     return non_negative; // Success
/// }
///
/// int main() {
///     my_function(0); // compilation error since the return value is not used (C++17)
///
///     auto result1 = my_function(1); // compilation error since 'result1' is unused
///
///     auto result2 = my_function(2); // OK, result is used
///     if (result2) {
///     }
///
///     my_function(3).value(); // OK, throws on error
///     my_function(4).or_else(utils::throw_error<>).value(); // OK, clearer error handling
///     my_function(5).map([](auto i) { return std::to_string(i); }).value_or(std::string("NEG")); // OK
///
///     utils::ignore(my_function(6)); // OK, but not recommended
///
///     auto result3 = my_function(7); // OK, but not recommended
///     ltb::utils::handled(result3);
///
///     return 0;
/// }
///
/// \endcode
template < typename T, typename E = ::ltb::utils::Error >
using Expected = tl::expected< T, E >;

auto success( ) -> Expected< void >;

template < typename E >
auto success( ) -> Expected< void, E >
{
    return { };
}

/// \brief A wrapper around `utils::Expected`s to show the result has
///        been handled and to suppress the related compiler warning.
template < typename... Args >
void handled( Args&&... )
{
}

} // namespace ltb::utils
