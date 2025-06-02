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
    Contributors: MAM, SAN
    Creation Date:  March 25th, 2025
*/

#pragma once

#include <include/endian.h>
#include <include/concepts/arithmetic_concept.h>
#include <include/conversion_string.h>

#include <type_traits>
#include <cstdint>
#include <limits>
#include <string>
#include <cmath>


namespace lime
{

    //=========================================================================
    [[maybe_unused]]
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

    template <std::integral T> class dynamic_value;

    template <typename T>
    concept dynamic_value_concept = std::is_same_v<T, dynamic_value<typename T::value_type>>;

    template <typename T>
    static inline constexpr bool is_dynamic_value_v {dynamic_value_concept<T>};

    #pragma pack(push, 1)
    template <std::integral T = std::int64_t>
    class dynamic_value final
    {
    public:

        using value_type = T;

        constexpr dynamic_value() = default;
        constexpr dynamic_value(dynamic_value_concept auto const &);
        constexpr dynamic_value & operator = (dynamic_value_concept auto const &);
        constexpr dynamic_value(dynamic_value_concept auto &&);
        constexpr dynamic_value & operator = (dynamic_value_concept auto &&);
        
        constexpr dynamic_value
        (
            std::integral auto,
            std::uint8_t
        );

        constexpr dynamic_value
        (
            std::floating_point auto,
            std::uint8_t
        );

        constexpr dynamic_value
        (
            std::floating_point auto
        );

        template <std::floating_point T_>
        constexpr operator T_() const;

        constexpr explicit operator bool() const {return (value_ != 0);}

        void set_precision
        (
            std::uint8_t
        );

        constexpr auto get_underlying_value() const;
        constexpr auto get_precision() const;

	    constexpr dynamic_value & operator -= (dynamic_value other) noexcept {
            align_precision(other);
            value_ -= other.value_;
            return *this;
        }
	    constexpr dynamic_value & operator += (dynamic_value other) noexcept {
            align_precision(other);
            value_ += other.value_;
            return *this;
        }
	    constexpr dynamic_value & operator *= (dynamic_value other) noexcept {
            align_precision(other);
            value_ *= other.value_;
            value_ /= pow_10(other.precision_);
            return *this;
        }
	    constexpr dynamic_value & operator /= (dynamic_value other) noexcept {
            align_precision(other);
            value_ *= pow_10(other.precision_);
            value_ /= other.value_;
            return *this;
        }
        
        constexpr dynamic_value & operator -= (arithmetic_concept auto n) noexcept {
            *this -= dynamic_value(n, get_precision());
            return *this;
        }
        
        constexpr dynamic_value & operator += (arithmetic_concept auto n) noexcept {
            *this += dynamic_value(n, get_precision());
            return *this;
        }
        
        constexpr dynamic_value & operator *= (arithmetic_concept auto n) noexcept {
            *this *= dynamic_value(n, get_precision());
            return *this;
        }
        
        constexpr dynamic_value & operator /= (arithmetic_concept auto n) noexcept {
            *this /= dynamic_value(n, get_precision());
            return *this;
        }
        
        constexpr auto operator <=>
        (
            dynamic_value_concept auto
        ) const noexcept;

        constexpr auto operator == 
        (
            dynamic_value_concept auto
        ) const noexcept;

	    constexpr auto operator !=
		(
			dynamic_value_concept auto
		) const noexcept;

    private:

        template <std::integral> friend class dynamic_value;

        value_type      value_{0};
        std::uint8_t    precision_{0};
        
        void align_precision(dynamic_value & other) noexcept;
    };
    #pragma pack(pop)
	
	template <std::integral T>
	constexpr dynamic_value<T> operator - (dynamic_value<T> lhs, dynamic_value<T> rhs) noexcept { lhs -= rhs; return lhs; }

	template <std::integral T>
	constexpr dynamic_value<T> operator + (dynamic_value<T> lhs, dynamic_value<T> rhs) noexcept { lhs += rhs; return lhs; }

	template <std::integral T>
	constexpr dynamic_value<T> operator * (dynamic_value<T> lhs, dynamic_value<T> rhs) noexcept { lhs *= rhs; return lhs; }

	template <std::integral T>
	constexpr dynamic_value<T> operator / (dynamic_value<T> lhs, dynamic_value<T> rhs) noexcept { lhs /= rhs; return lhs; }

	
	template <std::integral To>
	constexpr auto up_cast
	(
		dynamic_value_concept auto p
	)
	{
		using value_type = std::decay_t<decltype(p)>::value_type;
		if constexpr (std::numeric_limits<To>::max() > std::numeric_limits<value_type>::max()) {
			return dynamic_value<To>(p.get_underlying_value() / pow_10(p.get_precision()), p.get_precision());
		} else {
			return p;
		}
	}

/** arithmetics of the different underlying types ============================ */
	template <std::integral Lhs, std::integral Rhs>
	requires (!std::same_as<Lhs, Rhs>)
	constexpr dynamic_value<std::conditional_t<(std::numeric_limits<Lhs>::max() > std::numeric_limits<Rhs>::max()), Lhs, Rhs>>
	operator +
	(
		dynamic_value<Lhs> lhs,
		dynamic_value<Rhs> rhs
	) noexcept
	{
		auto res {up_cast<Rhs>(lhs)};
		res += up_cast<Lhs>(rhs);
		return res;
	}

	template <std::integral Lhs, std::integral Rhs>
	requires (!std::same_as<Lhs, Rhs>)
	constexpr dynamic_value<std::conditional_t<(std::numeric_limits<Lhs>::max() > std::numeric_limits<Rhs>::max()), Lhs, Rhs>>
	operator -
	(
		dynamic_value<Lhs> lhs,
		dynamic_value<Rhs> rhs
	) noexcept
	{
		auto res {up_cast<Rhs>(lhs)};
		res -= up_cast<Lhs>(rhs);
		return res;
	}

	template <std::integral Lhs, std::integral Rhs>
	requires (!std::same_as<Lhs, Rhs>)
	constexpr dynamic_value<std::conditional_t<(std::numeric_limits<Lhs>::max() > std::numeric_limits<Rhs>::max()), Lhs, Rhs>>
	operator *
	(
		dynamic_value<Lhs> lhs,
		dynamic_value<Rhs> rhs
	) noexcept
	{
		auto res {up_cast<Rhs>(lhs)};
		res *= up_cast<Lhs>(rhs);
		return res;
	}

	template <std::integral Lhs, std::integral Rhs>
	requires (!std::same_as<Lhs, Rhs>)
	constexpr dynamic_value<std::conditional_t<(std::numeric_limits<Lhs>::max() > std::numeric_limits<Rhs>::max()), Lhs, Rhs>>
	operator /
	(
		dynamic_value<Lhs> lhs,
		dynamic_value<Rhs> rhs
	) noexcept
	{
		auto res {up_cast<Rhs>(lhs)};
		res /= up_cast<Lhs>(rhs);
		return res;
	}


    //=========================================================================
    [[maybe_unused]]
    static std::string to_string
    (
        dynamic_value_concept auto dynamic_value
    )
    {
        if (dynamic_value.get_precision() == 0)
            return std::to_string(dynamic_value.get_underlying_value());

        auto s = lime::to_string(dynamic_value.get_underlying_value());
        while (s.size() <= dynamic_value.get_precision()) {
            s = '0' + s;
        }
        s.resize(s.size() + 1);
        auto beg = s.size() - 1;
        auto end = (beg - dynamic_value.get_precision());
        for (auto i = beg; i >= end; --i) {
            s[i] = (i == end) ? '.' : s[i - 1];
        }
        while(!s.empty() && (s.back() == '0' || s.back() == '.')) {
            s.pop_back();
        }
        return s;
    }
    
} // namespace lime


//=============================================================================
template <std::integral T>
constexpr lime::dynamic_value<T>::dynamic_value
(
    dynamic_value_concept auto const & other
):
    value_(other.value_),
    precision_(other.precision_)
{
}


//=============================================================================
template <std::integral T>
constexpr auto lime::dynamic_value<T>::operator =
(
    dynamic_value_concept auto const & other
) -> dynamic_value &
{
    value_ = other.value_;
    precision_ = other.precision_;
    return *this;
}


//=============================================================================
template <std::integral T>
constexpr lime::dynamic_value<T>::dynamic_value
(
    dynamic_value_concept auto && other
):
    value_(other.value_),
    precision_(other.precision_)
{
}


//=============================================================================
template <std::integral T>
constexpr auto lime::dynamic_value<T>::operator =
(
    dynamic_value_concept auto && other
) -> dynamic_value &
{
    value_ = other.value_;
    precision_ = other.precision_;
    return *this;
}


//=============================================================================
template <std::integral T>
constexpr lime::dynamic_value<T>::dynamic_value
(
    std::integral auto value,
    std::uint8_t precision
):
    value_(T(pow_10(precision)) * value),
    precision_(precision)
{
}


//=============================================================================
template <std::integral T>
constexpr lime::dynamic_value<T>::dynamic_value
(
    std::floating_point auto value,
    std::uint8_t precision
):
    value_(T(pow_10(precision)) * value),
    precision_(precision)
{
}


//=============================================================================
template <std::integral T>
constexpr lime::dynamic_value<T>::dynamic_value
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
template <std::integral T>
template <std::floating_point T_>
constexpr lime::dynamic_value<T>::operator T_
(
) const
{
    return ((T_)value_ / T(pow_10(precision_)));
}


//=============================================================================
template <std::integral T>
void lime::dynamic_value<T>::set_precision
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
template <std::integral T>
void lime::dynamic_value<T>::align_precision
(
    dynamic_value & other
) noexcept
{
    auto const max_precision {std::max(precision_, other.precision_)};
    max_precision == precision_ ?
        other.set_precision(max_precision) :
        set_precision(max_precision);
}


//=============================================================================
template <std::integral T>
constexpr inline auto lime::dynamic_value<T>::get_underlying_value
(
) const
{
    return value_;
}


//=============================================================================
template <std::integral T>
constexpr inline auto lime::dynamic_value<T>::get_precision
(
) const
{
    return precision_;
}


//=============================================================================
template <std::integral T>
constexpr inline auto lime::dynamic_value<T>::operator <=>
(
    dynamic_value_concept auto other
) const noexcept
{
    if (precision_ == other.precision_)
        return (value_ <=> other.value_);
    if (precision_ < other.precision_)
        return ((T(pow_10(other.precision_ - precision_)) * value_) <=> other.value_);
    return (value_ <=> (T(pow_10(precision_ - other.precision_)) * other.value_));
}


//=============================================================================
template <std::integral T>
constexpr inline auto lime::dynamic_value<T>::operator ==
(
    dynamic_value_concept auto other
) const noexcept
{
    if (precision_ == other.precision_)
        return (value_ == other.value_);
    if (precision_ < other.precision_)
        return ((pow_10(other.precision_ - precision_) * value_) == other.value_);
    return (value_ == (pow_10(precision_ - other.precision_) * other.value_));
}

//=============================================================================
template <std::integral T>
constexpr inline auto lime::dynamic_value<T>::operator !=
(
	dynamic_value_concept auto other
) const noexcept
{
	return !(*this == other);
}


//=========================================================================
[[maybe_unused]]
static inline constexpr auto lime::byte_swap
(
    lime::dynamic_value_concept auto value
) -> std::decay_t<decltype(value)>
{
    using lime::byte_swap;
    using return_type = std::decay_t<decltype(value)>;
    auto const p {byte_swap(value.get_precision())};
    double const _ {1.0 * byte_swap(value.get_underlying_value()) / pow_10(p)};
    return return_type(_, p);
}
