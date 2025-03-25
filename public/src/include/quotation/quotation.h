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

#include "./shares.h"
#include "./price.h"
#include "./fixed_price.h"

#include <type_traits>
#include <concepts>
#include <cstdint>
#include <iostream>


namespace lime
{

    enum class quotation_type : std::uint8_t
    {
        bid             = 1,
        buy             = bid,
        ask             = 2,
        offer           = ask,
        sell            = ask,

        trade           = 3,
        volume          = 4,
        strike          = 5,
        imbalance       = 6,
        round_lot       = 7,
        abstract        = 8 
    };

    template <typename T>
    concept quotable_concept = (shares_concept<T> || price_concept<T>);

    template <quotation_type T0, quotable_concept T1> struct quotation;

    template <typename T>
    concept quotation_concept = std::is_same_v<T, quotation<T::quotation_type, typename T::value_type>>;


    //=========================================================================
    template <quotation_type T0, quotable_concept T1>
    class quotation
    {
    public:

        static auto constexpr quotation_type    = T0;
        using value_type                        = T1;

        constexpr quotation() = default;

        template <quotable_concept T_> constexpr quotation(quotation<quotation_type, T_> const &);

        template <quotable_concept T_> constexpr quotation & operator = (quotation<quotation_type, T_> const &);

        template <quotable_concept T_> constexpr quotation(quotation<quotation_type, T_> &&);

        template <quotable_concept T_> constexpr quotation & operator = (quotation<quotation_type, T_> &&);

        template <numeric_concept ... Ts> constexpr quotation(Ts ...);
 
        constexpr quotation(value_type const &);
        
        constexpr quotation(value_type &&);

        constexpr std::strong_ordering operator <=> (quotation_concept auto const &);

        constexpr bool operator == (quotation_concept auto const &);

        constexpr auto get() const;

        template <quotable_concept T_> constexpr quotation operator + (quotation<quotation_type, T_> const & other) const{auto value = value_; return {value + other.value_};}
        template <quotable_concept T_> constexpr quotation & operator += (quotation<quotation_type, T_> const & other) {value_ += other.value_; return *this;}
        constexpr quotation operator + (numeric_concept auto n) const{auto value = value_; return {value + n};}
        constexpr quotation & operator += (numeric_concept auto n) {value_ += n; return *this;}


        template <quotable_concept T_> constexpr quotation operator - (quotation<quotation_type, T_> const & other) const{auto value = value_; return {value - other.value_};}
        template <quotable_concept T_> constexpr quotation & operator -= (quotation<quotation_type, T_> const & other) {value_ -= other.value_; return *this;}
        constexpr quotation operator - (numeric_concept auto n) const{auto value = value_; return {value - n};}
        constexpr quotation & operator -= (numeric_concept auto n) {value_ -= n; return *this;}


        template <quotable_concept T_> constexpr quotation operator * (quotation<quotation_type, T_> const & other) const{auto value = value_; return {value * other.value_};}
        template <quotable_concept T_> constexpr quotation & operator *= (quotation<quotation_type, T_> const & other) {value_ *= other.value_; return *this;}
        constexpr quotation operator * (numeric_concept auto n) const{auto value = value_; return {value * n};}
        constexpr quotation & operator *= (numeric_concept auto n) {value_ *= n; return *this;}


        template <quotable_concept T_> constexpr quotation operator / (quotation<quotation_type, T_> const & other) const{auto value = value_; return {value / other.value_};}
        template <quotable_concept T_> constexpr quotation & operator /= (quotation<quotation_type, T_> const & other) {value_ /= other.value_; return *this;}
        constexpr quotation operator / (numeric_concept auto n) const {auto value = value_; return {value / n};}
        constexpr quotation & operator /= (numeric_concept auto n) {value_ /= n; return *this;}

    private:

        template <lime::quotation_type, quotable_concept> friend class quotation;
        template <lime::quotation_type> friend constexpr auto quotation_cast(quotation_concept auto const &);

        friend std::ostream & operator << 
        (
            std::ostream & stream,
            quotation const & quotation 
        )
        {
            stream << to_string(quotation.value_);
            return stream;
        }

        value_type value_;
    };


    //=========================================================================
    // price quotation types
    template <quotation_type T0, std::integral T1> using price_quotation = quotation<T0, price<T1>>;

    template <std::integral T> using bid_price = price_quotation<quotation_type::bid, T>;
    template <std::integral T> using ask_price = price_quotation<quotation_type::ask, T>;
    template <std::integral T> using trade_price = price_quotation<quotation_type::trade, T>;
    template <std::integral T> using strike_price = price_quotation<quotation_type::strike, T>;
    template <std::integral T> using abstract_price = price_quotation<quotation_type::abstract, T>;


    //=========================================================================
    // shares quotation types
    template <quotation_type T0, std::integral T1> using shares_quotation = quotation<T0, shares<T1>>;

    template <std::integral T> using bid_shares = shares_quotation<quotation_type::bid, T>;
    template <std::integral T> using ask_shares = shares_quotation<quotation_type::ask, T>;
    template <std::integral T> using trade_shares = shares_quotation<quotation_type::trade, T>;
    template <std::integral T> using round_lot_shares = shares_quotation<quotation_type::round_lot, T>;
    template <std::integral T> using volume_shares = shares_quotation<quotation_type::volume, T>;
    template <std::integral T> using abstract_shares = shares_quotation<quotation_type::abstract, T>;


    //=========================================================================
    template <quotation_type T>
    [[__maybe_unused__]]
    static constexpr auto quotation_cast
    (
        quotation_concept auto const & input
    )
    {
        using input_type = std::decay_t<decltype(input)>;
        using output_type = quotation<T, typename input_type::value_type>;
        return output_type(input.value_);
    }


    //=========================================================================
    [[__maybe_unused__]]
    static constexpr inline auto byte_swap
    (
        quotation_concept auto const & source
    ) -> std::decay_t<decltype(source)>
    {
        using lime::byte_swap;
        return std::decay_t<decltype(source)>(byte_swap(source.get()));
    }    


    //=========================================================================
    [[__maybe_unused__]]
    static constexpr inline std::string to_string
    (
        quotation_concept auto source
    )
    {
        return to_string(source.get());
    }    

} // namespace lime


//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
template <lime::quotable_concept T_>
constexpr lime::quotation<T0, T1>::quotation
(
    quotation<quotation_type, T_> const & other
):
    value_(other.value_)
{
}


//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
template <lime::quotable_concept T_>
constexpr auto lime::quotation<T0, T1>::operator = 
(
    quotation<quotation_type, T_> const & other
) -> quotation & 
{
    value_ = other.value_; 
    return *this;
}


//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
template <lime::quotable_concept T_>
constexpr lime::quotation<T0, T1>::quotation
(
    quotation<quotation_type, T_> && other
):
    value_(other.value_)
{
}


//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
template <lime::quotable_concept T_>
constexpr auto lime::quotation<T0, T1>::operator = 
(
    quotation<quotation_type, T_> && other
) -> quotation & 
{
    value_ = other.value_; 
    return *this;
}


//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
template <lime::numeric_concept ... Ts> 
constexpr lime::quotation<T0, T1>::quotation
(
    Ts ... args
):
    value_(std::forward<Ts>(args) ...)
{
}


//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
constexpr lime::quotation<T0, T1>::quotation
(
    value_type const & value
):
    value_(value)
{
}


//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
constexpr lime::quotation<T0, T1>::quotation
(
    value_type && value
):
    value_(value)
{
}


//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
constexpr std::strong_ordering lime::quotation<T0, T1>::operator <=> 
(
    quotation_concept auto const & other
)
{
    if (auto n = (value_ <=> other.value_); n <= 0)
        return (n == 0) ? std::strong_ordering::equivalent : std::strong_ordering::less;
    return std::strong_ordering::greater;
}


//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
constexpr bool lime::quotation<T0, T1>::operator == 
(
    quotation_concept auto const & other
)
{
    return (value_ == other.value_);
}


//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
constexpr auto lime::quotation<T0, T1>::get
(
) const
{
    return value_;
}
