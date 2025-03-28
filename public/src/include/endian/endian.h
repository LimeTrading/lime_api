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

#include "./byte_swap.h"
#include "./endian_swap.h"

#include <concepts>
#include <type_traits>
#include <bit>
#include <utility>


namespace lime
{

    template <typename data_type, std::endian endian_type>
    class endian;


    //=========================================================================
    template <typename T>
    concept endian_concept = std::is_same_v<T, endian<typename T::underlying_type, T::type>>;


    //==============================================================================
    template <typename data_type, std::endian endian_type>
    class endian
    {
    public:

        using underlying_type = data_type;
        static auto constexpr type = endian_type;

        template <typename, std::endian> friend class endian;

        constexpr endian() = default;

        template <typename T>
        constexpr endian
        (
            T &&
        );

        template <typename T0, typename ... Ts>
        requires ((sizeof ... (Ts) > 1) || !std::is_same_v<data_type, T0>)
        constexpr endian
        (
            T0 &&,
            Ts && ...
        );

        constexpr endian
        (
            endian const &
        ) = default;

        constexpr endian
        (
            endian &&
        ) = default;

        constexpr endian & operator =
        (
            endian const &
        ) = default;

        constexpr endian & operator =
        (
            endian &&
        ) = default;

        template <typename T>
        requires (std::is_convertible_v<underlying_type, T>)
        constexpr endian & operator =
        (
            T &&
        );

        template <typename T>
        requires (std::is_convertible_v<underlying_type, T>)
        constexpr operator T
        (
        ) const
        {
            return {get()};
        }

        constexpr underlying_type get() const;

        constexpr auto operator <=> 
        (
            endian_concept auto const & other
        ) const
        {
            return (get() <=> other.get());
        }

        constexpr auto operator <=> 
        (
            underlying_type other
        ) const
        {
            return (get() <=> other);
        }

        constexpr auto operator == 
        (
            endian_concept auto const & other
        ) const
        {
            return (get() == other.get());
        }

        constexpr auto operator ==
        (
            underlying_type other
        ) const
        {
            return (get() == other);
        }

    private:

        underlying_type  value_;
    };


    template <typename data_type> using big_endian = endian<data_type, std::endian::big>;
    template <typename data_type> using network_order = big_endian<data_type>;
    template <typename data_type> using little_endian = endian<data_type, std::endian::little>;
    template <typename data_type> using native_endian = endian<data_type, std::endian::native>;
    template <typename data_type> using host_order = native_endian<data_type>;

} // namespace lime


//==============================================================================
template <typename data_type, std::endian endian_type>
template <typename T0, typename ... Ts>
requires ((sizeof ... (Ts) > 1) || !std::is_same_v<data_type, T0>)
constexpr lime::endian<data_type, endian_type>::endian
(
    // ctor constructs data_type from one or more arguments
    // where, in the event of only one argument, the first argument can not be endian<data_type>
    T0 && arg0,
    Ts && ... args
):
    endian(data_type(std::forward<T0>(arg0), std::forward<Ts>(args) ...))
{
}


//==============================================================================
template <typename data_type, std::endian endian_type>
template <typename T>
//requires (std::is_convertible_v<data_type, T>)
constexpr lime::endian<data_type, endian_type>::endian
(
    T && input
)
{
    value_ = endian_swap<std::endian::native, endian_type>(underlying_type(std::forward<T>(input)));
}


//==============================================================================
template <typename data_type, std::endian endian_type>
template <typename T>
requires (std::is_convertible_v<data_type, T>)
constexpr auto lime::endian<data_type, endian_type>::operator =
(
    T && input
) -> endian &
{
    value_ = endian_swap<std::endian::native, endian_type>(underlying_type(std::forward<T>(input)));
    return *this;
}


//==============================================================================
template <typename data_type, std::endian endian_type>
constexpr auto lime::endian<data_type, endian_type>::get
(
) const -> underlying_type
{
    return endian_swap<endian_type, std::endian::native>(value_);
}
