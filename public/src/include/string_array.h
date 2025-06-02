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
    Author: MAM
    Creation Date:  March 31, 2025
*/

#pragma once

#include <include/endian.h>
#include <include/trim.h>

#include <array>
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <concepts>
#include <span>
#include <algorithm>


namespace lime
{

    template <std::size_t N, char F> class string_array;

    template <typename T>
    concept string_array_concept = std::is_same_v<T, string_array<T::max_size, T::fill>>;

    #pragma pack(push, 1)
    template <std::size_t N, char F = ' '>
    class string_array final
    {
    public:

        static auto constexpr fill = F;
        static auto constexpr max_size = N;
        using value_type = std::array<char, max_size>;
        using size_type = std::size_t;

        string_array();

        string_array
        (
            value_type const &
        );

        string_array
        (
            std::string const &
        );

        template <typename T>
        requires (std::is_same_v<std::decay_t<T>, std::string_view> || std::is_same_v<std::decay_t<T>, std::span<char const>>)
        string_array
        (
            T
        );

        ~string_array() = default;

        string_array(string_array_concept auto const & other) requires (other.capacity() <= N);
        string_array & operator = (string_array_concept auto const & other) requires (other.capacity() <= N);
        string_array(string_array_concept auto && other) requires (other.capacity() <= N);
        string_array & operator = (string_array_concept auto && other) requires (other.capacity() <= N);

        std::string_view get() const;

        constexpr operator std::string_view() const;

        constexpr operator std::span<char const> const() const;

        constexpr size_type size() const;

        static constexpr size_type capacity();

        static constexpr char get_fill_character();

        constexpr auto empty() const;

        constexpr auto begin() const{return value_.begin();}

        constexpr auto end() const{return value_.end();}

        constexpr auto begin(){return value_.begin();}

        constexpr auto end(){return value_.end();}

        void clear();

        constexpr auto operator <=>
        (
            string_array const & other
        ) const
        {
            return strncmp(value_.data(), other.value_.data(), value_.size());
        }
        
    private:

        value_type  value_;

    }; // string_array
    #pragma pack(pop)


} // namespace lime


namespace std
{
    template <lime::string_array_concept T>
    struct hash<T>
    {
        std::size_t operator()
        (
            T const & stringArray
        ) const
        {
            return std::hash<std::string_view>()(trim(stringArray));
        }
    };
}


//=============================================================================
[[__maybe_unused__]]
static std::ostream & operator <<
(
    std::ostream & stream,
    lime::string_array_concept auto const & stringArray
)
{
    stream << stringArray.get();
    return stream;
}


//=============================================================================
template <std::size_t N, char F>
lime::string_array<N, F>::string_array
(
    string_array_concept auto const & other
) requires (other.capacity() <= N)
{
    auto otherSize = other.size();
    auto fillLocation = std::copy_n(other.begin(), otherSize, begin());
    if (fillLocation < end())
        std::fill_n(fillLocation, (capacity() - otherSize), fill);
}


//=============================================================================
template <std::size_t N, char F>
lime::string_array<N, F>::string_array
(
    string_array_concept auto && other
) requires (other.capacity() <= N)
{
    auto otherSize = other.size();
    auto fillLocation = std::copy_n(other.begin(), otherSize, begin());
    if (fillLocation < end())
        std::fill_n(fillLocation, (capacity() - otherSize), fill);
}


//=============================================================================
template <std::size_t N, char F>
auto lime::string_array<N, F>::operator =
(
    string_array_concept auto const & other
) -> string_array & requires (other.capacity() <= N)
{
    if constexpr (std::is_same_v<string_array, std::decay_t<decltype(other)>>)
    {
        if (this == &other)
            return *this;
    }
    auto otherSize = other.size();
    auto fillLocation = std::copy_n(other.begin(), otherSize, begin());
    if (fillLocation < end())
        std::fill_n(fillLocation, (capacity() - otherSize), fill);
    return *this;
}


//=============================================================================
template <std::size_t N, char F>
auto lime::string_array<N, F>::operator =
(
    string_array_concept auto && other
) -> string_array & requires (other.capacity() <= N)
{
    if constexpr (std::is_same_v<string_array, std::decay_t<decltype(other)>>)
    {
        if (this == &other)
            return *this;
    }
    auto otherSize = other.size();
    auto fillLocation = std::copy_n(other.begin(), otherSize, begin());
    if (fillLocation < end())
        std::fill_n(fillLocation, (capacity() - otherSize), fill);
    return *this;
}


//=============================================================================
template <std::size_t N, char F>
lime::string_array<N, F>::string_array
(
)
{
    std::fill_n(value_.data(), value_.size(), fill);
}


//=============================================================================
template <std::size_t N, char F>
lime::string_array<N, F>::string_array
(
    std::string const & value
):
    string_array(std::span(value.c_str(), value.size()))
{
}


//=============================================================================
template <std::size_t N, char F>        
template <typename T>
requires (std::is_same_v<std::decay_t<T>, std::string_view> || std::is_same_v<std::decay_t<T>, std::span<char const>>)
lime::string_array<N, F>::string_array
(
    T value
)
{
    auto bytesToCopy = std::min(value.size(), value_.size());
    std::copy_n(value.data(), bytesToCopy, value_.data());
    if (bytesToCopy < value_.size())
        std::fill_n(value_.data() + bytesToCopy, value_.size() - bytesToCopy, fill);
}


//=============================================================================
template <std::size_t N, char F>
lime::string_array<N, F>::string_array
(
    value_type const & value
):
    value_(value)
{
}


//=============================================================================
template <std::size_t N, char F>
void lime::string_array<N, F>::clear
(
)
{
    std::fill_n(value_.data(), value_.size(), fill);
}


//=============================================================================
template <std::size_t N, char F>
std::string_view lime::string_array<N, F>::get
(
) const
{
    return {value_.data(), size()};
}


//=============================================================================
template <std::size_t N, char F>
constexpr lime::string_array<N, F>::operator std::string_view
(
) const
{
    return {value_.data(), size()};
}


//=============================================================================
template <std::size_t N, char F>
constexpr lime::string_array<N, F>::operator std::span<char const> const
(
) const
{
    return {value_.data(), size()};
}


//=============================================================================
template <std::size_t N, char F>
auto constexpr lime::string_array<N, F>::size
(
) const -> size_type
{
    for (auto i = 0ull; i < capacity(); ++i)
        if (value_[i] == fill)
            return i;
    return capacity();
}


//=============================================================================
template <std::size_t N, char F>
auto constexpr lime::string_array<N, F>::empty
(
) const
{
    return (size() == 0);
}


//=============================================================================
template <std::size_t N, char F>
auto constexpr lime::string_array<N, F>::capacity
(
) -> size_type
{
    return N;
}


//=============================================================================
template <std::size_t N, char F>
auto constexpr lime::string_array<N, F>::get_fill_character
(
) -> char
{
    return fill;
}
