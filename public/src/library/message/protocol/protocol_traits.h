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

#include "./protocol_name.h"
#include "./version.h"

#include <concepts>
#include <type_traits>


namespace lime::message
{

    namespace details
    {
    
        template <protocol_name T0, version V, typename T1>
        struct protocol_traits
        {
            using message_indicator = T1;
            static constexpr auto name_{T0};
            static constexpr auto version_{V};
        };  
    
    }

    template <protocol_name T0, version V, typename T1, typename ... optional_traits>
    struct protocol_traits : details::protocol_traits<T0, V, T1>, optional_traits ...
    {
    };
    

    template <typename T>
    concept protocol_traits_concept = std::is_base_of_v<details::protocol_traits<T::name_, T::version_, typename T::message_indicator>, T>;
    
} // namespace lime::message


//=============================================================================
static constexpr auto operator <=>
(
    lime::message::protocol_traits_concept auto const & first,
    lime::message::protocol_traits_concept auto const & second
) noexcept
{
    if constexpr (auto a = (first.name_ <=> second.name_); a != 0)
        return a;
    return (first.version_ <=> second.version_);
}


//=============================================================================
static constexpr auto operator ==
(
    lime::message::protocol_traits_concept auto const & first,
    lime::message::protocol_traits_concept auto const & second
) noexcept
{
    if constexpr (first.name_ != second.name_)
        return false;
    return (first.version_ == second.version_);
}

