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

#include "./currency.h"

#include <include/endian.h>
#include <include/fundamental.h>
#include <include/conversion_string.h>

#include <string>
#include <cstdint>
#include <type_traits>
#include <concepts>


namespace lime
{

    template <numeric_concept T, currency Ccy> class price;

    template <typename T>
    concept price_concept = std::is_same_v<T, price<typename T::value_type, T::ccy>>;
    
    template <numeric_concept T = std::int64_t, currency Ccy = ccy::no_currency>
    class price final
    {
    public:

        using value_type = T;
        static constexpr auto ccy = Ccy;

        constexpr price() = default;

        constexpr price(price_concept auto const &);
        constexpr price & operator = (price_concept auto const &);
        constexpr price(price_concept auto &&);
        constexpr price & operator = (price_concept auto &&);
        
        template <typename... Args>
        requires std::constructible_from<value_type, Args...>
        constexpr explicit price(Args && ...args ) : value_(std::forward<Args>(args)...){}
        
        constexpr explicit operator bool() const noexcept;
        
        constexpr value_type get() const noexcept {return value_;}

        constexpr auto get_underlying_value() const noexcept {return value_.get_underlying_value();}

        void set_decimal_precision
        (
            std::uint8_t value
        ) requires (dynamic_value_concept<value_type>)
        {
            value_.set_precision(value);
        }

	    constexpr price & operator -= (price_concept auto other) noexcept { value_ -= other.value_; return *this;}
	    constexpr price & operator += (price_concept auto other) noexcept { value_ += other.value_; return *this;}
	    constexpr price & operator *= (price_concept auto other) noexcept { value_ *= other.value_; return *this;}
	    constexpr price & operator /= (price_concept auto other) noexcept { value_ /= other.value_; return *this;}
        
        constexpr price & operator -= (numeric_concept auto n) noexcept {
            if constexpr (is_dynamic_value_v<price::value_type> && is_integral_v<std::decay_t<decltype(n)>>)
            {
                *this -= price(n, value_.get_precision()); return *this;
            }
            else if constexpr (is_dynamic_value_v<price::value_type> && is_fixed_value_v<std::decay_t<decltype(n)>>)
            {
                *this -= price(static_cast<double>(n)); return *this;
            }
            else
            {
                *this -= price(n); return *this;
            }
        }
        constexpr price & operator += (numeric_concept auto n) noexcept {
            if constexpr (is_dynamic_value_v<price::value_type> && is_integral_v<std::decay_t<decltype(n)>>)
            {
                *this += price(n, value_.get_precision()); return *this;
            }
            else if constexpr (is_dynamic_value_v<price::value_type> && is_fixed_value_v<std::decay_t<decltype(n)>>)
            {
                *this += price(static_cast<double>(n)); return *this;
            }
            else
            {
                *this += price(n); return *this;
            }
        }
        constexpr price & operator *= (numeric_concept auto n) noexcept {
            if constexpr (is_dynamic_value_v<price::value_type> && is_integral_v<std::decay_t<decltype(n)>>)
            {
                *this *= price(n, value_.get_precision()); return *this;
            }
            else if constexpr (is_dynamic_value_v<price::value_type> && is_fixed_value_v<std::decay_t<decltype(n)>>)
            {
                *this *= price(static_cast<double>(n)); return *this;
            }
            else
            {
                *this *= price(n); return *this;
            }
        }
        constexpr price & operator /= (numeric_concept auto n) noexcept {
            if constexpr (is_dynamic_value_v<price::value_type> && is_integral_v<std::decay_t<decltype(n)>>)
            {
                *this /= price(n, value_.get_precision()); return *this;
            }
            else if constexpr (is_dynamic_value_v<price::value_type> && is_fixed_value_v<std::decay_t<decltype(n)>>)
            {
                *this /= price(static_cast<double>(n)); return *this;
            }
            else
            {
                *this /= price(n); return *this;
            }
        }
        
        constexpr auto operator <=> (price_concept auto other) const noexcept {return (value_ <=> other.value_);}
        constexpr auto operator == (price_concept auto other) const noexcept {return (value_ == other.value_);}
	    constexpr auto operator != (price_concept auto other) const noexcept {return (value_ != other.value_);}
		
    private:

        template <numeric_concept, currency> friend class price;

        value_type value_;

    }; // class price<>
    
    template <numeric_concept T, currency Ccy>
    constexpr price<T, Ccy> operator - (price<T, Ccy> lhs, price<T, Ccy> rhs) noexcept { lhs -= rhs; return lhs;}

	template <numeric_concept T, currency Ccy>
	constexpr price<T, Ccy> operator + (price<T, Ccy> lhs, price<T, Ccy> rhs) noexcept { lhs += rhs; return lhs; }
 
	template <numeric_concept T, currency Ccy>
	constexpr price<T, Ccy> operator * (price<T, Ccy> lhs, price<T, Ccy> rhs) noexcept { lhs *= rhs; return lhs; }

	template <numeric_concept T, currency Ccy>
	constexpr price<T, Ccy> operator / (price<T, Ccy> lhs, price<T, Ccy> rhs) noexcept { lhs /= rhs; return lhs; }


	template <numeric_concept To>
	constexpr auto up_cast
	(
		price_concept auto p
	)
	{
        using price_type = std::decay_t<decltype(p)>;
        using original_value_type = typename price_type::value_type;
        if constexpr (std::numeric_limits<To>::max() > std::numeric_limits<original_value_type>::max()) {
            if constexpr (is_dynamic_value_v<To> && is_integral_v<original_value_type>)
            {
                return price<To, price_type::ccy>(p.get());
            }
            else if constexpr (is_dynamic_value_v<To> && is_fixed_value_v<original_value_type>)
            {
                return price<To, price_type::ccy>(static_cast<double>(p.get()));
            }
            else
            {
                return price<To, price_type::ccy>(p);
            }
		} else {
			return p;
		}
	}


	/** arithmetics of the different underlying types ============================ */
	template <numeric_concept Lhs, numeric_concept Rhs, currency Ccy>
	requires (!std::same_as<Lhs, Rhs>)
    constexpr price<typename std::decay_t<decltype(get_max_numeric_limit<Lhs, Rhs>())>::type, Ccy>
	operator +
	(
		price<Lhs, Ccy> lhs,
		price<Rhs, Ccy> rhs
	) noexcept
	{
		auto res {up_cast<Rhs>(lhs)};
		res += up_cast<Lhs>(rhs);
		return res;
	}
    
    template <numeric_concept Lhs, numeric_concept Rhs, currency Ccy>
	requires (!std::same_as<Lhs, Rhs>)
	constexpr price<typename std::decay_t<decltype(get_max_numeric_limit<Lhs, Rhs>())>::type, Ccy>
	operator -
	(
        price<Lhs, Ccy> lhs,
        price<Rhs, Ccy> rhs
	) noexcept
	{
		auto res {up_cast<Rhs>(lhs)};
		res -= up_cast<Lhs>(rhs);
		return res;
	}
    
    template <numeric_concept Lhs, numeric_concept Rhs, currency Ccy>
	requires (!std::same_as<Lhs, Rhs>)
    constexpr price<typename std::decay_t<decltype(get_max_numeric_limit<Lhs, Rhs>())>::type, Ccy>
	operator *
	(
        price<Lhs, Ccy> lhs,
        price<Rhs, Ccy> rhs
	) noexcept
	{
		auto res {up_cast<Rhs>(lhs)};
		res *= up_cast<Lhs>(rhs);
		return res;
	}
    
    template <numeric_concept Lhs, numeric_concept Rhs, currency Ccy>
	requires (!std::same_as<Lhs, Rhs>)
    constexpr price<typename std::decay_t<decltype(get_max_numeric_limit<Lhs, Rhs>())>::type, Ccy>
	operator /
	(
        price<Lhs, Ccy> lhs,
        price<Rhs, Ccy> rhs
	) noexcept
	{
		auto res {up_cast<Rhs>(lhs)};
		res /= up_cast<Lhs>(rhs);
		return res;
	}


	//=========================================================================
    [[maybe_unused]]
    static constexpr inline auto byte_swap
    (
		price_concept auto source
    )
    {
	    using lime::byte_swap;
        return decltype(source){byte_swap(source.get())};
    }


    //=========================================================================
    [[maybe_unused]]
    static std::string to_string
    (
		price_concept auto source
    )
    {
        std::string res;
        res += lime::to_string(source.get());
        res += ' ';
        res += lime::to_string(source.ccy);
        return res;
    }

} // namespace lime


//=========================================================================
[[maybe_unused]]
static std::ostream & operator << 
(
    std::ostream & stream,
    lime::price_concept auto price
)
{
    stream << lime::to_string(price);
    return stream;
}


//=============================================================================
template <lime::numeric_concept T, lime::currency Ccy>
constexpr lime::price<T, Ccy>::price
(
	price_concept auto const & other
):
	value_(other.value_)
{
}


//=============================================================================
template <lime::numeric_concept T, lime::currency Ccy>
constexpr auto lime::price<T, Ccy>::operator =
(
	price_concept auto const & other
) -> price &
{
    value_ = other.value_;
    return *this;
}


//=============================================================================
template <lime::numeric_concept T, lime::currency Ccy>
constexpr lime::price<T, Ccy>::price
(
	price_concept auto && other
):
    value_(other.value_)
{
}


//=============================================================================
template <lime::numeric_concept T, lime::currency Ccy>
constexpr auto lime::price<T, Ccy>::operator =
(
	price_concept auto && other
) -> price &
{
    value_ = other.value_;
    return *this;
}


//=============================================================================
template <lime::numeric_concept T, lime::currency Ccy>
constexpr lime::price<T, Ccy>::operator bool()
const noexcept
{
    if constexpr (is_arithmetic_v<value_type>) {
        return (value_ != 0);
    }
    else if constexpr (is_fundamental_v<value_type>) {
        return value_.operator bool();
    }
    else {
        return false;
    }
}


//=============================================================================
namespace std {
    
    template<lime::price_concept T>
    struct hash<T>
    {
        auto operator()
        (
            T source
        ) const noexcept
        {
            auto hash_combine = []
            (
                std::size_t& seed, auto const& val
            )
            {
                seed ^= std::hash<std::decay_t<decltype(val)>>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            };
            
            std::size_t h {0u};

            hash_combine(h, source.get());
            hash_combine(h, T::ccy);

            return h;
        }
    };
    
}
