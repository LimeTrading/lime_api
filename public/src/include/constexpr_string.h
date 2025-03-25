/*
MIT License

Copyright (c) 2025 Lime Trading

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
    Contributors: MAM
    Creation Date:  March 25th, 2025
*/

#pragma once

#include <cstdint>
#include <type_traits>
#include <concepts>
#include <string_view>
#include <algorithm>


namespace lime
{

    template <std::size_t N>
    struct constexpr_string
    {
        using element_type = char;
        using value_type = element_type[N];

        constexpr constexpr_string
        (
            element_type const (&value)[N]
        ) noexcept
        {
            std::copy_n(value, N, value_);
        }

        constexpr value_type const & get() const noexcept;

        constexpr operator value_type const & () const noexcept;

        constexpr operator std::string_view const() const noexcept;

        static auto constexpr size() noexcept {return N;}

        value_type value_;
    };


    //=========================================================================
    template <typename T>
    concept constexpr_string_concept = std::is_same_v<T, constexpr_string<T::size()>>;

} // namespace lime


//=============================================================================
template <std::size_t N>
constexpr auto lime::constexpr_string<N>::get
(
) const noexcept -> value_type const & 
{
    return value_;
}


//=============================================================================
template <std::size_t N>
constexpr lime::constexpr_string<N>::operator value_type const & 
(
) const noexcept
{
    return value_;
}


//=============================================================================
template <std::size_t N>
constexpr lime::constexpr_string<N>::operator std::string_view const
(
) const noexcept
{
    return value_;
}


//=============================================================================
static constexpr auto operator <=>
(
    lime::constexpr_string_concept auto const & first,
    lime::constexpr_string_concept auto const & second
) noexcept
{
    return (std::string_view(first) <=> std::string_view(second));
}


//=============================================================================
static constexpr auto operator ==
(
    lime::constexpr_string_concept auto const & first,
    lime::constexpr_string_concept auto const & second
) noexcept
{
    return (std::string_view(first) == std::string_view(second));
}