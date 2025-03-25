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

#include <include/constexpr_string.h>

#include <concepts>
#include <type_traits>


namespace lime::message
{

    template <std::size_t N>
    struct protocol_name : 
        public constexpr_string<N>
    {
        constexpr protocol_name
        (
            char const (&value)[N]
        ) noexcept :
            constexpr_string<N>(value)
        {
        }
    };


    template <typename T>
    concept protocol_name_concept = std::is_same_v<T, protocol_name<T::size()>>;

} // namespace lime::message


//=============================================================================
static constexpr auto operator <=>
(
    lime::message::protocol_name_concept auto const & first,
    lime::message::protocol_name_concept auto const & second
) noexcept
{
    return (std::string_view(first) <=> std::string_view(second));
}


//=============================================================================
static constexpr auto operator ==
(
    lime::message::protocol_name_concept auto const & first,
    lime::message::protocol_name_concept auto const & second
) noexcept
{
    return (std::string_view(first) == std::string_view(second));
}
