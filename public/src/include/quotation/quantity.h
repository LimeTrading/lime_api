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

#include <include/endian.h>
#include <include/fundamental.h>
#include <include/conversion_string.h>

#include <string>
#include <cstdint>
#include <type_traits>


namespace lime
{

    template <numeric_concept T> class quantity;

    template <typename T>
    concept quantity_concept = std::is_same_v<T, quantity<typename T::value_type>>;


    template <numeric_concept T>
    class quantity final
    {
    public:

        using value_type = T;

        constexpr quantity() = default;

        constexpr quantity(quantity_concept auto const &);
        constexpr quantity & operator = (quantity_concept auto const &);
        constexpr quantity(quantity_concept auto &&);
        constexpr quantity & operator = (quantity_concept auto &&);
        
        template <typename... Args>
        requires std::constructible_from<value_type, Args...>
        constexpr explicit quantity(Args && ...args ) : value_(std::forward<Args>(args)...){}
        
        constexpr explicit operator bool() const noexcept;
        
        constexpr value_type get() const{return value_;}

	    constexpr quantity & operator -= (quantity_concept auto other) noexcept { value_ -= other.value_; return *this;}
	    constexpr quantity & operator += (quantity_concept auto other) noexcept { value_ += other.value_; return *this;}
	    constexpr quantity & operator *= (quantity_concept auto other) noexcept { value_ *= other.value_; return *this;}
	    constexpr quantity & operator /= (quantity_concept auto other) noexcept { value_ /= other.value_; return *this;}
        
        constexpr quantity & operator -= (numeric_concept auto n) noexcept {
            if constexpr (is_dynamic_value_v<quantity::value_type> && is_integral_v<std::decay_t<decltype(n)>>)
            {
                *this -= quantity(n, value_.get_precision()); return *this;
            }
            else if constexpr (is_dynamic_value_v<quantity::value_type> && is_fixed_value_v<std::decay_t<decltype(n)>>)
            {
                *this -= quantity(static_cast<double>(n)); return *this;
            }
            else
            {
                *this -= quantity(n); return *this;
            }
        }
        constexpr quantity & operator += (numeric_concept auto n) noexcept {
            if constexpr (is_dynamic_value_v<quantity::value_type> && is_integral_v<std::decay_t<decltype(n)>>)
            {
                *this += quantity(n, value_.get_precision()); return *this;
            }
            else if constexpr (is_dynamic_value_v<quantity::value_type> && is_fixed_value_v<std::decay_t<decltype(n)>>)
            {
                *this += quantity(static_cast<double>(n)); return *this;
            }
            else
            {
                *this += quantity(n); return *this;
            }
        }
        constexpr quantity & operator *= (numeric_concept auto n) noexcept {
            if constexpr (is_dynamic_value_v<quantity::value_type> && is_integral_v<std::decay_t<decltype(n)>>)
            {
                *this *= quantity(n, value_.get_precision()); return *this;
            }
            else if constexpr (is_dynamic_value_v<quantity::value_type> && is_fixed_value_v<std::decay_t<decltype(n)>>)
            {
                *this *= quantity(static_cast<double>(n)); return *this;
            }
            else
            {
                *this *= quantity(n); return *this;
            }
        }
        constexpr quantity & operator /= (numeric_concept auto n) noexcept {
            if constexpr (is_dynamic_value_v<quantity::value_type> && is_integral_v<std::decay_t<decltype(n)>>)
            {
                *this /= quantity(n, value_.get_precision()); return *this;
            }
            else if constexpr (is_dynamic_value_v<quantity::value_type> && is_fixed_value_v<std::decay_t<decltype(n)>>)
            {
                *this /= quantity(static_cast<double>(n)); return *this;
            }
            else
            {
                *this /= quantity(n); return *this;
            }
        }
        
        constexpr auto operator <=> (quantity_concept auto other) const noexcept {return (value_ <=> other.value_);}
        constexpr auto operator == (quantity_concept auto other) const noexcept {return (value_ == other.value_);}
	    constexpr auto operator != (quantity_concept auto other) const noexcept {return (value_ != other.value_);}
		
    private:

        template <numeric_concept> friend class quantity;

        value_type value_;

    }; // class quantity<>

	template <numeric_concept T>
	constexpr quantity<T> operator - (quantity<T> lhs, quantity<T> rhs) noexcept { lhs -= rhs; return lhs; }

	template <numeric_concept T>
	constexpr quantity<T> operator + (quantity<T> lhs, quantity<T> rhs) noexcept { lhs += rhs; return lhs; }

	template <numeric_concept T>
	constexpr quantity<T> operator * (quantity<T> lhs, quantity<T> rhs) noexcept { lhs *= rhs; return lhs; }

	template <numeric_concept T>
	constexpr quantity<T> operator / (quantity<T> lhs, quantity<T> rhs) noexcept { lhs /= rhs; return lhs; }


	template <numeric_concept To>
	constexpr auto up_cast
	(
		quantity_concept auto q
	)
	{
		using original_value_type = std::decay_t<decltype(q)>::value_type;
		if constexpr (std::numeric_limits<To>::max() > std::numeric_limits<original_value_type>::max()) {

            if constexpr (is_dynamic_value_v<To> && is_integral_v<original_value_type>)
            {
                return quantity<To>(q.get(), 0);
            }
            else if constexpr (is_dynamic_value_v<To> && is_fixed_value_v<original_value_type>)
            {
                return quantity<To>(static_cast<double>(q.get()));
            }
            else
            {
                return quantity<To>(q);
            }

		} else {
			return q;
		}
	}


	/** arithmetics of the different underlying types ============================ */
	template <numeric_concept Lhs, numeric_concept Rhs>
	requires (!std::same_as<Lhs, Rhs>)
    constexpr quantity<typename std::decay_t<decltype(get_max_numeric_limit<Lhs, Rhs>())>::type>
	operator +
	(
		quantity<Lhs> lhs,
		quantity<Rhs> rhs
	) noexcept
	{
		auto res {up_cast<Rhs>(lhs)};
		res += up_cast<Lhs>(rhs);
		return res;
	}

	template <numeric_concept Lhs, numeric_concept Rhs>
	requires (!std::same_as<Lhs, Rhs>)
    constexpr quantity<typename std::decay_t<decltype(get_max_numeric_limit<Lhs, Rhs>())>::type>
	operator -
	(
		quantity<Lhs> lhs,
		quantity<Rhs> rhs
	) noexcept
	{
		auto res {up_cast<Rhs>(lhs)};
		res -= up_cast<Lhs>(rhs);
		return res;
	}

	template <numeric_concept Lhs, numeric_concept Rhs>
	requires (!std::same_as<Lhs, Rhs>)
    constexpr quantity<typename std::decay_t<decltype(get_max_numeric_limit<Lhs, Rhs>())>::type>
	operator *
	(
		quantity<Lhs> lhs,
		quantity<Rhs> rhs
	) noexcept
	{
		auto res {up_cast<Rhs>(lhs)};
		res *= up_cast<Lhs>(rhs);
		return res;
	}

	template <numeric_concept Lhs, numeric_concept Rhs>
	requires (!std::same_as<Lhs, Rhs>)
    constexpr quantity<typename std::decay_t<decltype(get_max_numeric_limit<Lhs, Rhs>())>::type>
	operator /
	(
		quantity<Lhs> lhs,
		quantity<Rhs> rhs
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
		quantity_concept auto source
    )
    {
	    using lime::byte_swap;
        return decltype(source){byte_swap(source.get())};
    }


    //=========================================================================
    [[maybe_unused]]
    static std::string to_string
    (
		quantity_concept auto source
    )
    {
        return lime::to_string(source.get());
    }

} // namespace lime


//=========================================================================
[[maybe_unused]]
static std::ostream & operator << 
(
    std::ostream & stream,
    lime::quantity_concept auto q
)
{
    stream << lime::to_string(q);
    return stream;
}


//=============================================================================
template <lime::numeric_concept T>
constexpr lime::quantity<T>::quantity
(
	quantity_concept auto const & other
):
	value_(other.value_)
{
}


//=============================================================================
template <lime::numeric_concept T>
constexpr auto lime::quantity<T>::operator =
(
	quantity_concept auto const & other
) -> quantity &
{
    value_ = other.value_;
    return *this;
}


//=============================================================================
template <lime::numeric_concept T>
constexpr lime::quantity<T>::quantity
(
	quantity_concept auto && other
):
    value_(other.value_)
{
}


//=============================================================================
template <lime::numeric_concept T>
constexpr auto lime::quantity<T>::operator =
(
	quantity_concept auto && other
) -> quantity &
{
    value_ = other.value_;
    return *this;
}

//=============================================================================
template <lime::numeric_concept T>
constexpr lime::quantity<T>::operator bool()
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
    
    template<lime::quantity_concept T>
    struct hash<T>
    {
        auto operator()(T source) const noexcept { return source.get(); }
    };
    
}
