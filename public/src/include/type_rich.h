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
#include <include/endian.h>


namespace lime
{

    template <typename T1, std::integral T2, T2 D = {}>
    class type_rich
    {
    public:

        using tag_type = T1;
        using value_type = T2;
        static auto constexpr default_value = D;

        constexpr type_rich() = default;
        explicit constexpr type_rich(value_type value):value_(value){}
        constexpr type_rich(type_rich const &) = default;
        constexpr type_rich & operator = (type_rich const &) = default;
        constexpr type_rich(type_rich &&) = default;
        constexpr type_rich & operator = (type_rich &&) = default;

        constexpr auto operator <=> (type_rich const &) const = default;
        constexpr auto get() const{return value_;}

    private:

        value_type  value_{default_value};
    };


    template <typename T>
    concept type_rich_concept = std::is_same_v<T, type_rich<typename T::tag_type, typename T::value_type, T::default_value>>;


    //==============================================================================
    [[__maybe_unused__]]
    static inline constexpr auto byte_swap
    (
        type_rich_concept auto value
    ) -> std::decay_t<decltype(value)>
    {
        using lime::byte_swap;
        return std::decay_t<decltype(value)>(byte_swap(value.get()));
    }

} // namespace lime