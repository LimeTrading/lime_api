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

#include "./dynamic_value.h"

namespace lime
{

    template <std::uint32_t N, std::integral T> class fixed_value;

    template <typename T>
    concept fixed_value_concept = std::is_same_v<T, fixed_value<T::precision, typename T::value_type>>;
    
    template <typename T>
    static inline constexpr bool is_fixed_value_v {fixed_value_concept<T>};

#pragma pack(push, 1)
    template <std::uint32_t N = 4, std::integral T = std::int64_t>
    class fixed_value final
    {
    public:

        using value_type = T;
        static auto constexpr precision = N;

        constexpr fixed_value() = default;
        constexpr fixed_value(fixed_value_concept auto const &);
        constexpr fixed_value & operator = (fixed_value_concept auto const &);
        constexpr fixed_value(fixed_value_concept auto &&);
        constexpr fixed_value & operator = (fixed_value_concept auto &&);
        
        constexpr fixed_value
        (
            arithmetic_concept auto
        );

        constexpr fixed_value
        (
            dynamic_value_concept auto const &
        );

        template <std::floating_point T_>
        constexpr operator T_() const;

        constexpr explicit operator bool() const{return (value_ != 0);}

        void set_precision
        (
            std::uint8_t
        );

        constexpr auto get_underlying_value() const;
        static constexpr auto get_precision();

	    constexpr fixed_value & operator -= (fixed_value other) noexcept {value_ -= other.value_; return *this;}
	    constexpr fixed_value & operator += (fixed_value other) noexcept {value_ += other.value_; return *this;}
	    constexpr fixed_value & operator *= (fixed_value other) noexcept {value_ *= other.value_; value_ /= pow_10(get_precision()); return *this;}
	    constexpr fixed_value & operator /= (fixed_value other) noexcept {value_ *= pow_10(get_precision()); value_ /= other.value_; return *this;}
        
        constexpr fixed_value & operator -= (arithmetic_concept auto n) noexcept { *this -= fixed_value(n); return *this; }
        constexpr fixed_value & operator += (arithmetic_concept auto n) noexcept { *this += fixed_value(n); return *this; }
        constexpr fixed_value & operator *= (arithmetic_concept auto n) noexcept { *this *= fixed_value(n); return *this; }
        constexpr fixed_value & operator /= (arithmetic_concept auto n) noexcept { *this /= fixed_value(n); return *this; }

	    constexpr auto operator <=>
        (
            fixed_value_concept auto
        ) const noexcept;

        constexpr auto operator == 
        (
            fixed_value_concept auto
        ) const noexcept;

	    constexpr auto operator !=
		(
			fixed_value_concept auto
		) const noexcept;

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

        template <std::integral T_>
        operator dynamic_value<T_>() const;

    private:

        template <std::uint32_t, std::integral> friend class fixed_value;

        value_type value_ {0};
    };
    #pragma pack(pop)

    
	template <std::uint32_t N, std::integral T>
	constexpr fixed_value<N, T> operator - (fixed_value<N, T> lhs, fixed_value<N, T> rhs) noexcept { lhs -= rhs; return lhs;}

	template <std::uint32_t N, std::integral T>
	constexpr fixed_value<N, T> operator + (fixed_value<N, T> lhs, fixed_value<N, T> rhs) noexcept { lhs += rhs; return lhs; }

	template <std::uint32_t N, std::integral T>
	constexpr fixed_value<N, T> operator * (fixed_value<N, T> lhs, fixed_value<N, T> rhs) noexcept { lhs *= rhs; return lhs; }

	template <std::uint32_t N, std::integral T>
	constexpr fixed_value<N, T> operator / (fixed_value<N, T> lhs, fixed_value<N, T> rhs) noexcept { lhs /= rhs; return lhs; }


/** arithmetics of the different underlying types ============================ */

	template <std::uint32_t N1, std::integral T1, std::uint32_t N2, std::integral T2>
	requires requires {N1 != N2;}
	auto operator +
	(
		fixed_value<N1, T1> lhs,
		fixed_value<N2, T2> rhs
	) noexcept
	{
		if constexpr (N1 == N2) {
			return lhs + rhs;
		}
		if constexpr (N1 < N2) {
			fixed_value<N2, T2> res (lhs);
			res += rhs;
			return res;
		}
		if constexpr (N1 > N2) {
			fixed_value<N1, T1> res (lhs);
			res += rhs;
			return res;
		}
	}
	
	template <std::uint32_t N1, std::integral T1, std::uint32_t N2, std::integral T2>
	requires requires {N1 != N2;}
	auto operator -
	(
		fixed_value<N1, T1> lhs,
		fixed_value<N2, T2> rhs
		) noexcept
	{
		if constexpr (N1 == N2) {
			return lhs - rhs;
		}
		if constexpr (N1 < N2) {
			fixed_value<N2, T2> res (lhs);
			res -= rhs;
			return res;
		}
		if constexpr (N1 > N2) {
			fixed_value<N1, T1> res (lhs);
			res -= rhs;
			return res;
		}
	}

	template <std::uint32_t N1, std::integral T1, std::uint32_t N2, std::integral T2>
	requires requires {N1 != N2;}
	auto operator *
	(
		fixed_value<N1, T1> lhs,
		fixed_value<N2, T2> rhs
		) noexcept
	{
		if constexpr (N1 == N2) {
			return lhs * rhs;
		}
		if constexpr (N1 < N2) {
			fixed_value<N2, T2> res (lhs);
			res *= rhs;
			return res;
		}
		if constexpr (N1 > N2) {
			fixed_value<N1, T1> res (lhs);
			res *= rhs;
			return res;
		}
	}

	template <std::uint32_t N1, std::integral T1, std::uint32_t N2, std::integral T2>
	requires requires {N1 != N2;}
	auto operator /
	(
		fixed_value<N1, T1> lhs,
		fixed_value<N2, T2> rhs
	) noexcept
	{
		if constexpr (N1 == N2) {
			return lhs / rhs;
		}
		if constexpr (N1 < N2) {
			fixed_value<N2, T2> res (lhs);
			res /= rhs;
			return res;
		}
		if constexpr (N1 > N2) {
			fixed_value<N1, T1> res (lhs);
			res /= rhs;
			return res;
		}
	}

	

    //=========================================================================
    [[maybe_unused]]
    static std::string to_string
    (
        fixed_value_concept auto value
    )
    {
        auto s = lime::to_string(value.get_underlying_value());
        while (s.size() <= value.get_precision()) {
            s = '0' + s;
        }
        s.resize(s.size() + 1);
        auto beg = s.size() - 1;
        auto end = (beg - value.get_precision());
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
template <std::uint32_t N, std::integral T>
constexpr lime::fixed_value<N, T>::fixed_value
(
    fixed_value_concept auto const & other
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
template <std::uint32_t N, std::integral T>
constexpr lime::fixed_value<N, T>::fixed_value
(
    dynamic_value_concept auto const & other
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
template <std::uint32_t N, std::integral T>
constexpr auto lime::fixed_value<N, T>::operator =
(
    fixed_value_concept auto const & other
) -> fixed_value &
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
template <std::uint32_t N, std::integral T>
constexpr lime::fixed_value<N, T>::fixed_value
(
    fixed_value_concept auto && other
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
template <std::uint32_t N, std::integral T>
constexpr auto lime::fixed_value<N, T>::operator =
(
    fixed_value_concept auto && other
) -> fixed_value &
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
template <std::uint32_t N, std::integral T>
constexpr lime::fixed_value<N, T>::fixed_value
(
    arithmetic_concept auto value
):
    value_(value * pow_10(precision))
{
}


//=============================================================================
template <std::uint32_t N, std::integral T>
template <std::floating_point T_>
constexpr lime::fixed_value<N, T>::operator T_
(
) const
{
    return ((T_)value_ / pow_10(precision));
}


//=============================================================================
template <std::uint32_t N, std::integral T>
constexpr inline auto lime::fixed_value<N, T>::get_underlying_value
(
) const
{
    return value_;
}


//=============================================================================
template <std::uint32_t N, std::integral T>
auto constexpr lime::fixed_value<N, T>::get_precision
(
)
{
    return precision;
}


//=============================================================================
template <std::uint32_t N, std::integral T>
constexpr inline auto lime::fixed_value<N, T>::operator <=>
(
    fixed_value_concept auto other
) const noexcept
{
    if constexpr (get_precision() == other.get_precision())
        return (value_ <=> other.value_);
    if constexpr (get_precision() < other.get_precision())
        return ((T(pow_10(other.get_precision() - get_precision())) * value_) <=> other.value_);
    else
        return (value_ <=> (T(pow_10(get_precision() - other.get_precision())) * other.value_));
}


//=============================================================================
template <std::uint32_t N, std::integral T>
constexpr inline auto lime::fixed_value<N, T>::operator <=>
(
    dynamic_value_concept auto other
) const noexcept
{
    if (get_precision() == other.get_precision())
        return (value_ <=> other.get_underlying_value());
    return (fixed_value(other) <=> *this);
}


//=============================================================================
template <std::uint32_t N, std::integral T>
constexpr inline auto lime::fixed_value<N, T>::operator ==
(
    fixed_value_concept auto other
) const noexcept
{
    if constexpr (get_precision() == other.get_precision())
        return (value_ == other.get_underlying_value());
    if constexpr (get_precision() < other.get_precision())
        return ((pow_10(other.get_precision() - get_precision()) * value_) == other.value_);
    return (value_ == (pow_10(get_precision() - other.get_precision()) * other.value_));
}


//=============================================================================
template <std::uint32_t N, std::integral T>
constexpr inline auto lime::fixed_value<N, T>::operator ==
(
    dynamic_value_concept auto other
) const noexcept
{
    if (get_precision() == other.get_precision())
        return (value_ == other.get_underlying_value());
    return (fixed_value(other) == *this);
}

//=============================================================================
template <std::uint32_t N, std::integral T>
constexpr inline auto lime::fixed_value<N, T>::operator !=
(
	fixed_value_concept auto other
) const noexcept
{
	return !(*this == other);
}


//=============================================================================
template <std::uint32_t N, std::integral T>
constexpr inline auto lime::fixed_value<N, T>::operator !=
(
    dynamic_value_concept auto other
) const noexcept
{
	return !(*this == other);
}


//=============================================================================
template <std::uint32_t N, std::integral T>
template <std::integral T_>
lime::fixed_value<N, T>::operator dynamic_value<T_>
(
) const
{
    double const _ {*this};
    return {_, get_precision()};
}


//=========================================================================
template <std::uint32_t N, std::integral T>
[[maybe_unused]]
static inline constexpr auto lime::byte_swap
(
    fixed_value<N, T> source
) -> fixed_value<N, T> 
{
    return {lime::byte_swap(source.get_underlying_value())};
}
