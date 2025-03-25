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

#include "./price.h"
#include <include/concepts/numeric_concept.h>


namespace lime
{

    template <std::uint32_t N, std::unsigned_integral T> class fixed_price;

    template <typename T>
    concept fixed_price_concept = std::is_same_v<T, fixed_price<T::precision, typename T::value_type>>;


    #pragma pack(push, 1)
    template <std::uint32_t N = 4, std::unsigned_integral T = std::uint64_t>
    class fixed_price
    {
    public:

        using value_type = T;
        static auto constexpr precision = N;

        constexpr fixed_price() = default;
        constexpr fixed_price(fixed_price_concept auto const &);
        constexpr fixed_price & operator = (fixed_price_concept auto const &);
        constexpr fixed_price(fixed_price_concept auto &&);
        constexpr fixed_price & operator = (fixed_price_concept auto &&);

        constexpr fixed_price
        (
            std::floating_point auto,
            std::uint8_t
        );

        constexpr fixed_price
        (
            numeric_concept auto
        );

        constexpr fixed_price
        (
            price_concept auto
        );

        template <std::floating_point T_>
        constexpr operator T_() const;

        constexpr operator bool() const{return (value_ != 0);}

        void set_precision
        (
            std::uint8_t
        );

        constexpr auto get_underlying_value() const;
        static constexpr auto get_precision();

        constexpr auto operator <=> 
        (
            fixed_price_concept auto const &
        ) const;

        constexpr auto operator == 
        (
            fixed_price_concept auto const &
        ) const;

        constexpr auto operator <=> 
        (
            price_concept auto const &
        ) const;

        constexpr auto operator == 
        (
            price_concept auto const &
        ) const;

        template <std::unsigned_integral T_>
        operator price<T_>() const;

    private:

        template <std::uint32_t, std::unsigned_integral> friend class fixed_price;

        value_type value_;
    };
    #pragma pack(pop)


    //=========================================================================
    [[__maybe_unused__]]
    static inline constexpr auto byte_swap
    (
        fixed_price_concept auto price
    ) -> std::decay_t<decltype(price)>
    {
        using lime::byte_swap;
        return std::decay_t<decltype(price)>(byte_swap(price.get_underlying_value()));
    }    


    //=========================================================================
    [[__maybe_unused__]]
    static std::string to_string
    (
        fixed_price_concept auto price
    )
    {
        auto s = std::to_string(price.get_underlying_value());
        while (s.size() <= price.get_precision())
            s = '0' + s;
        s.resize(s.size() + 1);
        auto beg = s.size() - 1;
        auto end = (beg - price.get_precision());
        for (auto i = beg; i >= end; --i)
            s[i] = (i == end) ? '.' : s[i - 1];
        return s;
    }


    //=========================================================================
    [[__maybe_unused__]]
    static constexpr auto operator *
    (
        fixed_price_concept auto const & price,
        shares_concept auto const & shares
    )
    {
        return (price * shares.get());
    }


    //=========================================================================
    [[__maybe_unused__]]
    static constexpr auto operator *
    (
        shares_concept auto const & shares,
        fixed_price_concept auto const & price
    )
    {
        return (price * shares.get());
    }


} // namespace lime


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
constexpr lime::fixed_price<N, T>::fixed_price
(
    fixed_price_concept auto const & other
)
{
    auto value = other.value_;
    if constexpr (get_precision() != other.get_precision())
    {
        if (get_precision() < other.get_precision())
            value /= pow_10(other.get_precision() - get_precision());
        else
            value *= pow_10(get_precision() - other.get_precision());
    }
    value_ = value;
}


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
constexpr lime::fixed_price<N, T>::fixed_price
(
    price_concept auto other
)
{
    auto value = other.get_underlying_value();
    if (get_precision() != other.get_precision())
    {
        if (get_precision() < other.get_precision())
            value /= pow_10(other.get_precision() - get_precision());
        else
            value *= pow_10(get_precision() - other.get_precision());
    }
    value_ = value;
}


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
constexpr auto lime::fixed_price<N, T>::operator = 
(
    fixed_price_concept auto const & other
) -> fixed_price &
{
    auto value = other.value_;
    if constexpr (get_precision() != other.get_precision())
    {
        if (get_precision() < other.get_precision())
            value /= pow_10(other.get_precision() - get_precision());
        else
            value *= pow_10(get_precision() - other.get_precision());
    }
    value_ = value;
    return *this;
}


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
constexpr lime::fixed_price<N, T>::fixed_price
(
    fixed_price_concept auto && other
)
{
    auto value = other.value_;
    if constexpr (get_precision() != other.get_precision())
    {
        if (get_precision() < other.get_precision())
            value /= pow_10(other.get_precision() - get_precision());
        else
            value *= pow_10(get_precision() - other.get_precision());
    }
    value_ = value;
}


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
constexpr auto lime::fixed_price<N, T>::operator = 
(
    fixed_price_concept auto && other
) -> fixed_price &
{
    auto value = other.value_;
    if constexpr (get_precision() != other.get_precision())
    {
        if (get_precision() < other.get_precision())
            value /= pow_10(other.get_precision() - get_precision());
        else
            value *= pow_10(get_precision() - other.get_precision());
    }
    value_ = value;
    return *this;
}


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
constexpr lime::fixed_price<N, T>::fixed_price
(
    numeric_concept auto value
):
    value_(value * pow_10(precision))
{
}


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
template <std::floating_point T_>
constexpr lime::fixed_price<N, T>::operator T_
(
) const
{
    return ((T_)value_ / pow_10(precision));
}


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
constexpr inline auto lime::fixed_price<N, T>::get_underlying_value
(
) const
{
    return value_;
}


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
auto constexpr lime::fixed_price<N, T>::get_precision
(
)
{
    return precision;
}


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
constexpr inline auto lime::fixed_price<N, T>::operator <=> 
(
    fixed_price_concept auto const & other
) const
{
    if constexpr (get_precision() == other.get_precision())
        return (value_ <=> other.value_);
    if constexpr (get_precision() < other.get_precision())
        return ((T(pow_10(other.get_precision() - get_precision())) * value_) <=> other.value_);
    else
        return (value_ <=> (T(pow_10(get_precision() - other.get_precision())) * other.value_));
}


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
constexpr inline auto lime::fixed_price<N, T>::operator <=> 
(
    price_concept auto const & other
) const
{
    if (get_precision() == other.get_precision())
        return (value_ <=> other.get_underlying_value());
    return (fixed_price(other) <=> *this);
}


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
constexpr inline auto lime::fixed_price<N, T>::operator == 
(
    fixed_price_concept auto const & other
) const
{
    if constexpr (get_precision() == other.get_precision())
        return (value_ == other.get_underlying_value());
    if constexpr (get_precision() < other.get_precision())
        return ((pow_10(other.get_precision() - get_precision()) * value_) == other.value_);
    return (value_ == (pow_10(get_precision() - other.get_precision()) * other.value_));
}


//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
constexpr inline auto lime::fixed_price<N, T>::operator == 
(
    price_concept auto  const & other
) const
{
    if (get_precision() == other.get_precision())
        return (value_ == other.get_underlying_value());
    return (fixed_price(other) == *this);
}



//=============================================================================
template <std::uint32_t N, std::unsigned_integral T>
template <std::unsigned_integral T_>
lime::fixed_price<N, T>::operator price<T_>
(
) const
{
    return {value_, get_precision()};
}


//=========================================================================
[[__maybe_unused__]]
static std::ostream & operator << 
(
    std::ostream & stream,
    lime::fixed_price_concept auto const & price 
)
{
    stream << to_string(price);
    return stream;
}
