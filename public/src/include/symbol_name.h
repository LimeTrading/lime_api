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

#include <include/endian.h>

#include <array>
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <concepts>
#include <span>
#include <algorithm>


namespace lime
{

    #pragma pack(push, 1)
    template <std::size_t N, char F = ' '>
    class symbol_name
    {
    public:

        static auto constexpr fill = F;
        static auto constexpr max_size = N;
        using value_type = std::array<char, max_size>;
        using size_type = std::size_t;

        symbol_name();

        symbol_name(symbol_name const &) = default;
        symbol_name & operator = (symbol_name const &) = default;

        symbol_name
        (
            value_type const &
        );

        symbol_name
        (
            std::string const &
        );

        symbol_name
        (
            std::string_view const
        );

        symbol_name
        (
            std::span<char const>
        );

        std::string_view const get() const;

        constexpr operator std::string_view const() const;

        constexpr operator std::span<char const> const() const;

        constexpr size_type size();

        static constexpr size_type capacity();

        static constexpr char get_fill_character();

        constexpr auto empty() const;

        constexpr auto begin() const{return value_.begin();}

        constexpr auto end() const{return value_.end();}

        constexpr auto begin(){return value_.begin();}

        constexpr auto end(){return value_.end();}

        constexpr auto operator <=>
        (
            symbol_name const & other
        ) const
        {
            return strncmp(value_.data(), other.value_.data(), value_.size());
        }
        
    private:

        value_type  value_;

    }; // symbol
    #pragma pack(pop)


    template <typename T>
    concept symbol_name_concept = std::is_same_v<T, symbol_name<T::max_size, T::fill>>;


    //=========================================================================
    [[__maybe_unused__]]
    static inline auto byte_swap
    (
        symbol_name_concept auto const & symbolName
    ) -> std::decay_t<decltype(symbolName)>
    {
        return symbolName;
    }


} // namespace lime


//=============================================================================
template <std::size_t N, char F>
lime::symbol_name<N, F>::symbol_name
(
)
{
    std::fill_n(value_.data(), fill, value_.size());
}


//=============================================================================
template <std::size_t N, char F>
lime::symbol_name<N, F>::symbol_name
(
    std::string_view const value
)
{
    auto bytesToCopy = std::min(value.size(), value_.size());
    std::copy_n(value_.data(), bytesToCopy, value.data());
    if (bytesToCopy < value_.size())
        std::fill_n(value_.data() + bytesToCopy, fill, value_.size() - bytesToCopy);
}


//=============================================================================
template <std::size_t N, char F>
lime::symbol_name<N, F>::symbol_name
(
    std::string const & value
)
{
    auto bytesToCopy = std::min(value.size(), value_.size());
    std::copy_n(value_.data(), bytesToCopy, value.data());
    if (bytesToCopy < value_.size())
        std::fill_n(value_.data() + bytesToCopy, fill, value_.size() - bytesToCopy);
}


//=============================================================================
template <std::size_t N, char F>
lime::symbol_name<N, F>::symbol_name
(
    std::span<char const> value
)
{
    auto bytesToCopy = std::min(value.size(), value_.size());
    std::copy_n(value_.data(), bytesToCopy, value.data());
    if (bytesToCopy < value_.size())
        std::fill_n(value_.data() + bytesToCopy, fill, value_.size() - bytesToCopy);
}


//=============================================================================
template <std::size_t N, char F>
lime::symbol_name<N, F>::symbol_name
(
    value_type const & value
):
    value_(value)
{
}


//=============================================================================
template <std::size_t N, char F>
std::string_view const lime::symbol_name<N, F>::get
(
) const
{
    return {value_.data(), size()};
}


//=============================================================================
template <std::size_t N, char F>
constexpr lime::symbol_name<N, F>::operator std::string_view const
(
) const
{
    return {value_.data(), size()};
}


//=============================================================================
template <std::size_t N, char F>
constexpr lime::symbol_name<N, F>::operator std::span<char const> const
(
) const
{
    return {value_.data(), size()};
}


//=============================================================================
template <std::size_t N, char F>
auto constexpr lime::symbol_name<N, F>::size
(
) -> size_type
{
    for (auto i = 0; i < capacity(); ++i)
        if (value_[i] == fill)
            return i;
    return capacity;
}


//=============================================================================
template <std::size_t N, char F>
auto constexpr lime::symbol_name<N, F>::empty
(
) const
{
    return (size() == 0);
}


//=============================================================================
template <std::size_t N, char F>
auto constexpr lime::symbol_name<N, F>::capacity
(
) -> size_type
{
    return N;
}


//=============================================================================
template <std::size_t N, char F>
auto constexpr lime::symbol_name<N, F>::get_fill_character
(
) -> char
{
    return fill;
}
