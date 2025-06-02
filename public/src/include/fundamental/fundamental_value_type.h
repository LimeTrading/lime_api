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
    Contributors: MAM, SAN
    Creation Date:  March 25th, 2025
*/

#pragma once

#include "./dynamic_value.h"
#include "./fixed_value.h"

namespace lime {
    
    template <typename T> concept fundamental_type_concept = (fixed_value_concept<T> || dynamic_value_concept<T>);
    
    template <typename T> static inline constexpr bool is_fundamental_v {fundamental_type_concept<T>};
    
    template <typename T> concept numeric_concept = (arithmetic_concept<T> || fundamental_type_concept<T>);
    
    template <typename T> static inline constexpr bool is_numeric_v {numeric_concept<T>};
    
}//!namespace lime

//=========================================================================
[[maybe_unused]]
static std::ostream & operator <<
(
    std::ostream & stream,
    lime::fundamental_type_concept auto value
)
{
    stream << lime::to_string(value);
    return stream;
}

namespace std {
    
    template<lime::fundamental_type_concept T>
    struct numeric_limits<T> {
        using value_type = typename T::value_type;
        
        static constexpr bool is_specialized = true;
        
        static constexpr auto min() noexcept { return std::numeric_limits<value_type>::min(); }
        static constexpr auto max() noexcept { return std::numeric_limits<value_type>::max(); }
        static constexpr int digits = std::numeric_limits<value_type>::digits;
        static constexpr int digits10 = std::numeric_limits<value_type>::digits10;
        static constexpr bool is_signed = std::numeric_limits<value_type>::is_signed;
        static constexpr bool is_integer = std::numeric_limits<value_type>::is_integer;
        static constexpr bool is_exact = std::numeric_limits<value_type>::is_exact;
    };
    
    template<lime::fundamental_type_concept T>
    struct hash<T>
    {
        auto operator()(T source) const noexcept { return source.get(); }
    };
    
}//!namespace std

namespace lime {

//=========================================================================
    template<numeric_concept T0, numeric_concept T1>
    static constexpr auto get_max_numeric_limit
    (
    )
    {
        return
            std::type_identity<
                std::conditional_t<(std::numeric_limits<T0>::max() > std::numeric_limits<T1>::max()), T0, T1>
            >{};
    }
    
}//!namespace lime