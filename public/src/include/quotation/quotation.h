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
    Author: MAM, SAN
    Creation Date:  February 27, 2025
*/

#pragma once

#include "./notional_value.h"
#include "../conversion_string.h"


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
        abstract        = 8,
        mid             = 9,
        open_interest   = 10,

        open            = 11,
        close           = 12
    };


    static auto constexpr bid_quotation = quotation_type::bid;
    static auto constexpr ask_quotation = quotation_type::ask;
    static auto constexpr offer_quotation = quotation_type::offer;
    static auto constexpr mid_quotation = quotation_type::mid;
    static auto constexpr trade_quotation = quotation_type::trade;
    static auto constexpr strike_quotation = quotation_type::strike;
    static auto constexpr abstract_quotation = quotation_type::abstract;
    static auto constexpr round_lot_quotation = quotation_type::round_lot;
    static auto constexpr volume_quotation = quotation_type::volume;
    static auto constexpr open_interest_quotation = quotation_type::open_interest;
    static auto constexpr imbalance_quotation = quotation_type::imbalance;
    static auto constexpr open_quotation = quotation_type::open;
    static auto constexpr close_quotation = quotation_type::close;


    template <typename T>
    concept quotable_concept = (quantity_concept<T> || price_concept<T> || monetary_value_concept<T>);

    template <quotation_type T0, quotable_concept T1> struct quotation;

    template <typename T>
    concept quotation_concept = std::is_same_v<T, quotation<T::quotation_type, typename T::value_type>>;


    //=========================================================================
    template <quotation_type T0, quotable_concept T1>
    class quotation final
    {
    public:

        static auto constexpr quotation_type    = T0;
        using value_type                        = T1;

        constexpr quotation() = default;

        template <quotable_concept T_> constexpr quotation(quotation<quotation_type, T_> const &);

        template <quotable_concept T_> constexpr quotation & operator = (quotation<quotation_type, T_> const &);

        template <quotable_concept T_> constexpr quotation(quotation<quotation_type, T_> &&);

        template <quotable_concept T_> constexpr quotation & operator = (quotation<quotation_type, T_> &&);
        
        template <typename... Args>
        requires std::constructible_from<value_type, Args...>
        constexpr quotation(Args && ...args );
        
        constexpr quotation(value_type const &);
        
        constexpr quotation(value_type &&);

        constexpr std::strong_ordering operator <=> (quotation_concept auto const &) const noexcept;
		
        constexpr bool operator == (quotation_concept auto const &) const noexcept;

		constexpr bool operator != (quotation_concept auto const &) const noexcept;
        
        constexpr explicit operator bool() const noexcept;

        constexpr value_type get() const noexcept;

        constexpr quotation & operator += (quotation const & other) noexcept {value_ += other.value_; return *this;}
        constexpr quotation & operator -= (quotation const & other) noexcept {value_ -= other.value_; return *this;}
        constexpr quotation & operator *= (quotation const & other) noexcept {value_ *= other.value_; return *this;}
        constexpr quotation & operator /= (quotation const & other) noexcept {value_ /= other.value_; return *this;}
        
        constexpr quotation & operator += (numeric_concept auto n) noexcept { value_ += n; return *this; }
        constexpr quotation & operator -= (numeric_concept auto n) noexcept { value_ -= n; return *this; }
        constexpr quotation & operator *= (numeric_concept auto n) noexcept { value_ *= n; return *this; }
        constexpr quotation & operator /= (numeric_concept auto n) noexcept { value_ /= n; return *this; }
    
    private:

        template <lime::quotation_type, quotable_concept> friend class quotation;
        template <lime::quotation_type> friend constexpr auto quotation_cast(quotation_concept auto const &);
        
        value_type value_;
    };

	template <quotation_type T0, quotable_concept T1>
	constexpr quotation<T0, T1> operator - (quotation<T0, T1> lhs, quotation<T0, T1> rhs) noexcept {lhs -= rhs; return lhs;}

	template <quotation_type T0, quotable_concept T1>
	constexpr quotation<T0, T1> operator + (quotation<T0, T1> lhs, quotation<T0, T1> rhs) noexcept {lhs += rhs; return lhs;}

	template <quotation_type T0, quotable_concept T1>
	constexpr quotation<T0, T1> operator * (quotation<T0, T1> lhs, quotation<T0, T1> rhs) noexcept {lhs *= rhs; return lhs;}

	template <quotation_type T0, quotable_concept T1>
	constexpr quotation<T0, T1> operator / (quotation<T0, T1> lhs, quotation<T0, T1> rhs) noexcept {lhs /= rhs; return lhs;}

    
    //=========================================================================
    // quantity quotation types
    template <quotation_type T0, numeric_concept T1> using quantity_quotation = quotation<T0, quantity<T1>>;

    template <numeric_concept T> using bid_quantity = quantity_quotation<quotation_type::bid, T>;
    template <numeric_concept T> using ask_quantity = quantity_quotation<quotation_type::ask, T>;
    template <numeric_concept T> using offer_quantity = quantity_quotation<offer_quotation, T>;
    template <numeric_concept T> using trade_quantity = quantity_quotation<quotation_type::trade, T>;
    template <numeric_concept T> using round_lot_quantity = quantity_quotation<quotation_type::round_lot, T>;
    template <numeric_concept T> using volume_quantity = quantity_quotation<quotation_type::volume, T>;
    template <numeric_concept T> using abstract_quantity = quantity_quotation<quotation_type::abstract, T>;
    template <numeric_concept T> using open_interest_quantity = quantity_quotation<open_interest_quotation, T>;
    
    //=========================================================================
    // price quotation types
    template <quotation_type T0, lime::numeric_concept T1, lime::currency Ccy> using price_quotation = quotation<T0, price<T1, Ccy>>;
    
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using bid_price = price_quotation<bid_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using ask_price = price_quotation<ask_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using offer_price = price_quotation<offer_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using mid_price = price_quotation<mid_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using trade_price = price_quotation<trade_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using strike_price = price_quotation<strike_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using abstract_price = price_quotation<abstract_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using imbalance_price = price_quotation<imbalance_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using open_price = price_quotation<open_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using close_price = price_quotation<close_quotation, T, Ccy>;
    
    //=========================================================================
    // monetary value quotation types
    template <quotation_type T0, lime::numeric_concept T1, lime::currency Ccy> using monetary_value_quotation = quotation<T0, lime::monetary_value<T1, Ccy>>;
    
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using bid_monetary_value = monetary_value_quotation<bid_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using ask_monetary_value = monetary_value_quotation<ask_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using trade_monetary_value = monetary_value_quotation<trade_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using abstract_monetary_value = monetary_value_quotation<abstract_quotation, T, Ccy>;
    template <lime::numeric_concept T, lime::currency Ccy = default_currency> using imbalance_monetary_value = monetary_value_quotation<imbalance_quotation, T, Ccy>;
    
    
    //=========================================================================
    template <quotation_type T>
    [[maybe_unused]]
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
    [[maybe_unused]]
    static constexpr inline auto byte_swap
    (
        quotation_concept auto const & source
    ) -> std::decay_t<decltype(source)>
    {
        using lime::byte_swap;
        return std::decay_t<decltype(source)>(byte_swap(source.get()));
    }    


    //=========================================================================
    [[maybe_unused]]
    static std::string to_string
    (
        quotation_concept auto source
    )
    {
        return lime::to_string(source.get());
    }
    
} // namespace lime

template <lime::quotation_type T0, lime::quotable_concept T1>
std::ostream & operator <<
(
    std::ostream & stream,
    lime::quotation<T0, T1> const & quotation
)
{
    stream << lime::to_string(quotation.get());
    return stream;
}

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
template <typename... Args>
requires std::constructible_from<typename lime::quotation<T0, T1>::value_type, Args...>
constexpr lime::quotation<T0, T1>::quotation
(
    Args && ...args
):
    value_(std::forward<Args>(args) ...)
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
) const noexcept
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
) const noexcept
{
    return (value_ == other.value_);
}


//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
constexpr bool lime::quotation<T0, T1>::operator !=
(
	quotation_concept auto const & other
) const noexcept
{
	return !(*this == other);
}

//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
constexpr lime::quotation<T0, T1>::operator bool()
const noexcept
{
    return value_.operator bool();
}

//=============================================================================
template <lime::quotation_type T0, lime::quotable_concept T1>
constexpr lime::quotation<T0, T1>::value_type lime::quotation<T0, T1>::get
(
) const noexcept
{
    return value_;
}


//=============================================================================
namespace std {
    
    template<lime::quotation_concept T>
    struct hash<T>
    {
        auto operator()(T source) const noexcept { return source.get(); }
    };
    
}
