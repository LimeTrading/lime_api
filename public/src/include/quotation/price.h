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
#include <include/concepts/numeric_concept.h>

#include <concepts>
#include <type_traits>
#include <cstdint>
#include <limits>
#include <string>
#include <cmath>


namespace lime
{

    //=========================================================================
    [[__maybe_unused__]]
    static constexpr std::uint64_t pow_10
    (
        std::uint64_t n
    )
    {
        constexpr std::int64_t const pow_10_table[] =
        {
            1ull,
            10ull,
            100ull,
            1000ull,
            10000ull,
            100000ull,
            1000000ull,
            10000000ull,
            100000000ull,
            1000000000ull,
            10000000000ull,
            100000000000ull,
            1000000000000ull,
            10000000000000ull,
            100000000000000ull,
            1000000000000000ull,
            10000000000000000ull,
            100000000000000000ull,
            1000000000000000000ull
        };

        if (n < std::extent_v<decltype(pow_10_table)>) 
            return pow_10_table[n];
       return 0;
    }

    template <std::unsigned_integral T> class price;

    template <typename T>
    concept price_concept = std::is_same_v<T, price<typename T::value_type>>;


    #pragma pack(push, 1)
    template <std::unsigned_integral T = std::uint64_t>
    class price
    {
    public:

        using value_type = T;

        constexpr price() = default;
        constexpr price(price_concept auto const &);
        constexpr price & operator = (price_concept auto const &);
        constexpr price(price_concept auto &&);
        constexpr price & operator = (price_concept auto &&);
        
        constexpr price
        (
            std::integral auto,
            std::uint8_t
        );

        constexpr price
        (
            std::floating_point auto,
            std::uint8_t
        );

        constexpr price
        (
            std::floating_point auto
        );

        template <std::floating_point T_>
        constexpr operator T_() const;

        constexpr operator bool() const{return (value_ != 0);}

        void set_precision
        (
            std::uint8_t
        );

        constexpr auto get_underlying_value() const;
        constexpr auto get_precision() const;

        constexpr auto operator <=> 
        (
            price_concept auto const &
        ) const;

        constexpr auto operator == 
        (
            price_concept auto const &
        ) const;

    private:

        template <std::unsigned_integral> friend class price;

        value_type      value_{0};
        std::uint8_t    precision_{0};
    };
    #pragma pack(pop)
    

    //=========================================================================
    [[__maybe_unused__]]
    static inline constexpr auto byte_swap
    (
        price_concept auto price
    ) -> std::decay_t<decltype(price)>
    {
        using lime::byte_swap;
        return std::decay_t<decltype(price)>(byte_swap(price.get_underlying_value()), byte_swap(price.get_precision()));
    }    


    //=========================================================================
    [[__maybe_unused__]]
    static std::string to_string
    (
        price_concept auto price
    )
    {
        if (price.get_precision() == 0)
            return std::to_string(price.get_underlying_value());

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
        price_concept auto const & price,
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
        price_concept auto const & price
    )
    {
        return (price * shares.get());
    }


} // namespace lime


//=============================================================================
template <std::unsigned_integral T>
constexpr lime::price<T>::price
(
    price_concept auto const & other
):
    value_(other.value_),
    precision_(other.precision_)
{
}


//=============================================================================
template <std::unsigned_integral T>
constexpr auto lime::price<T>::operator = 
(
    price_concept auto const & other
) -> price &
{
    value_ = other.value_;
    precision_ = other.precision_;
    return *this;
}


//=============================================================================
template <std::unsigned_integral T>
constexpr lime::price<T>::price
(
    price_concept auto && other
):
    value_(other.value_),
    precision_(other.precision_)
{
}


//=============================================================================
template <std::unsigned_integral T>
constexpr auto lime::price<T>::operator = 
(
    price_concept auto  && other
) -> price &
{
    value_ = other.value_;
    precision_ = other.precision_;
    return *this;
}


//=============================================================================
template <std::unsigned_integral T>
constexpr lime::price<T>::price
(
    std::integral auto value,
    std::uint8_t precision
):
    value_(value),
    precision_(precision)
{
}


//=============================================================================
template <std::unsigned_integral T>
constexpr lime::price<T>::price
(
    std::floating_point auto value,
    std::uint8_t precision
):
    value_(T(pow_10(precision)) * value),
    precision_(precision)
{
}


//=============================================================================
template <std::unsigned_integral T>
constexpr lime::price<T>::price
(
    std::floating_point auto value
)
{
    while (value != T(value))
    {
        ++precision_;
        value *= 10;
    }
    value_ = value;
}


//=============================================================================
template <std::unsigned_integral T>
template <std::floating_point T_>
constexpr lime::price<T>::operator T_
(
) const
{
    return ((T_)value_ / T(pow_10(precision_)));
}


//=============================================================================
template <std::unsigned_integral T>
void lime::price<T>::set_precision
(
    std::uint8_t precision
)
{
    if (precision != precision_)
    {
        if (precision < precision_)
            value_ /= T(pow_10(precision_ - precision));
        else
            value_ *= T(pow_10(precision - precision_));
        precision_ = precision;
    }
}


//=============================================================================
template <std::unsigned_integral T>
constexpr inline auto lime::price<T>::get_underlying_value
(
) const
{
    return value_;
}


//=============================================================================
template <std::unsigned_integral T>
constexpr inline auto lime::price<T>::get_precision
(
) const
{
    return precision_;
}


//=============================================================================
template <std::unsigned_integral T>
constexpr inline auto lime::price<T>::operator <=> 
(
    price_concept auto  const & other
) const
{
    if (precision_ == other.precision_)
        return (value_ <=> other.value_);
    if (precision_ < other.precision_)
        return ((T(pow_10(other.precision_ - precision_)) * value_) <=> other.value_);
    return (value_ <=> (T(pow_10(precision_ - other.precision_)) * other.value_));
}


//=============================================================================
template <std::unsigned_integral T>
constexpr inline auto lime::price<T>::operator == 
(
    price_concept auto  const & other
) const
{
    if (precision_ == other.precision_)
        return (value_ == other.value_);
    if (precision_ < other.precision_)
        return ((pow_10(other.precision_ - precision_) * value_) == other.value_);
    return (value_ == (pow_10(precision_ - other.precision_) * other.value_));
}


//=========================================================================
[[__maybe_unused__]]
static std::ostream & operator << 
(
    std::ostream & stream,
    lime::price_concept auto const & price 
)
{
    stream << to_string(price);
    return stream;
}
