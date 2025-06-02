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
    Author: SAN
    Creation Date:  March 6, 2025
*/

#pragma once

#include "./quantity.h"
#include "./price.h"

namespace lime {

        template<numeric_concept T, currency Ccy>
        class monetary_value;

        template<typename T>
        concept monetary_value_concept = std::is_same_v<T, monetary_value<typename T::value_type, T::ccy>>;
        
        template<numeric_concept T, currency Ccy = ccy::no_currency>
        class monetary_value final {
        public:

            using value_type = T;
            static auto constexpr ccy = Ccy;

            constexpr monetary_value() = default;

            constexpr monetary_value(monetary_value_concept auto const &);

            constexpr monetary_value &operator=(monetary_value_concept auto const &);

            constexpr monetary_value(monetary_value_concept auto &&);

            constexpr monetary_value &operator=(monetary_value_concept auto &&);
            
            template <typename... Args>
            requires std::constructible_from<value_type, Args...>
            constexpr explicit monetary_value(Args && ...args ) : value_(std::forward<Args>(args)...){}

            template<numeric_concept T1, numeric_concept T2>
            constexpr explicit monetary_value (price<T1, Ccy>, quantity<T2>);
            
            constexpr explicit operator bool() const noexcept;
            
            template <std::floating_point T_>
            constexpr operator T_() const;

            constexpr value_type get() const{ return value_; }

            constexpr monetary_value &operator-=(monetary_value_concept auto other) noexcept{ value_ -= other.value_; return *this; }
            constexpr monetary_value &operator+=(monetary_value_concept auto other) noexcept{ value_ += other.value_; return *this; }
            constexpr monetary_value &operator*=(monetary_value_concept auto other) noexcept{ value_ *= other.value_; return *this; }
            constexpr monetary_value &operator/=(monetary_value_concept auto other) noexcept{ value_ /= other.value_; return *this; }

            constexpr monetary_value &operator-=(numeric_concept auto n) noexcept{
                if constexpr (is_dynamic_value_v<monetary_value::value_type> && is_integral_v<std::decay_t<decltype(n)>>)
                {
                    *this -= monetary_value(n, value_.get_precision()); return *this;
                }
                else if constexpr (is_dynamic_value_v<monetary_value::value_type> && is_fixed_value_v<std::decay_t<decltype(n)>>)
                {
                    *this -= monetary_value(static_cast<double>(n)); return *this;
                }
                else
                {
                    *this -= monetary_value(n); return *this;
                }
            }
            
            constexpr monetary_value &operator+=(numeric_concept auto n) noexcept{
                if constexpr (is_dynamic_value_v<monetary_value::value_type> && is_integral_v<std::decay_t<decltype(n)>>)
                {
                    *this += monetary_value(n, value_.get_precision()); return *this;
                }
                else if constexpr (is_dynamic_value_v<monetary_value::value_type> && is_fixed_value_v<std::decay_t<decltype(n)>>)
                {
                    *this += monetary_value(static_cast<double>(n)); return *this;
                }
                else
                {
                    *this += monetary_value(n); return *this;
                }
            }
            constexpr monetary_value &operator*=(numeric_concept auto n) noexcept{
                if constexpr (is_dynamic_value_v<monetary_value::value_type> && is_integral_v<std::decay_t<decltype(n)>>)
                {
                    *this *= monetary_value(n, value_.get_precision()); return *this;
                }
                else if constexpr (is_dynamic_value_v<monetary_value::value_type> && is_fixed_value_v<std::decay_t<decltype(n)>>)
                {
                    *this *= monetary_value(static_cast<double>(n)); return *this;
                }
                else
                {
                    *this *= monetary_value(n); return *this;
                }
            }
            constexpr monetary_value &operator/=(numeric_concept auto n) noexcept{
                if constexpr (is_dynamic_value_v<monetary_value::value_type> && is_integral_v<std::decay_t<decltype(n)>>)
                {
                    *this /= monetary_value(n, value_.get_precision()); return *this;
                }
                else if constexpr (is_dynamic_value_v<monetary_value::value_type> && is_fixed_value_v<std::decay_t<decltype(n)>>)
                {
                    *this /= monetary_value(static_cast<double>(n)); return *this;
                }
                else
                {
                    *this /= monetary_value(n); return *this;
                }
            }

            constexpr auto operator<=>(monetary_value_concept auto other) const noexcept { return (value_ <=> other.value_); }
            constexpr auto operator==(monetary_value_concept auto other) const noexcept { return (value_ == other.value_); }
            constexpr auto operator!=(monetary_value_concept auto other) const noexcept { return (value_ != other.value_); }

        private:

            template<numeric_concept, currency>
            friend class monetary_value;

            value_type value_;

        }; // class monetary_value<>

        template<numeric_concept T, currency Ccy>
        constexpr monetary_value<T, Ccy>
        operator-(monetary_value<T, Ccy> lhs, monetary_value<T, Ccy> rhs) noexcept{ lhs -= rhs; return lhs; }
        
        template<numeric_concept T, currency Ccy>
        constexpr monetary_value<T, Ccy>
        operator+(monetary_value<T, Ccy> lhs, monetary_value<T, Ccy> rhs) noexcept{ lhs += rhs; return lhs; }
        
        template<numeric_concept T, currency Ccy>
        constexpr monetary_value<T, Ccy>
        operator*(monetary_value<T, Ccy> lhs, monetary_value<T, Ccy> rhs) noexcept{ lhs *= rhs; return lhs; }
        
        template<numeric_concept T, currency Ccy>
        constexpr monetary_value<T, Ccy>
        operator/(monetary_value<T, Ccy> lhs, monetary_value<T, Ccy> rhs) noexcept{ lhs /= rhs; return lhs; }



        template<numeric_concept To>
        constexpr auto up_cast
        (
            monetary_value_concept auto m
        )
        {
            using monetary_value_type = std::decay_t<decltype(m)>;
            using original_value_type = typename monetary_value_type::value_type;
            if constexpr (std::numeric_limits<To>::max() > std::numeric_limits<original_value_type>::max()) {
                if constexpr (is_dynamic_value_v<To> && is_integral_v<original_value_type>)
                {
                    return monetary_value<To, monetary_value_type::ccy>(m.get());
                }
                else if constexpr (is_dynamic_value_v<To> && is_fixed_value_v<original_value_type>)
                {
                    return monetary_value<To, monetary_value_type::ccy>(static_cast<double>(m.get()));
                }
                else
                {
                    return monetary_value<To, monetary_value_type::ccy>(m);
                }
            } else {
                return m;
            }
        }


        /** arithmetics of the different underlying types ============================ */
        template <numeric_concept Lhs, numeric_concept Rhs, currency Ccy>
        requires (!std::same_as<Lhs, Rhs>)
        constexpr monetary_value<std::conditional_t<(std::numeric_limits<Lhs>::max() > std::numeric_limits<Rhs>::max()), Lhs, Rhs>, Ccy>
        operator +
        (
            monetary_value<Lhs, Ccy> lhs,
            monetary_value<Rhs, Ccy> rhs
        ) noexcept
        {
            auto res {up_cast<Rhs>(lhs)};
            res += up_cast<Lhs>(rhs);
            return res;
        }

        template <numeric_concept Lhs, numeric_concept Rhs, currency Ccy>
        requires (!std::same_as<Lhs, Rhs>)
        constexpr monetary_value<std::conditional_t<(std::numeric_limits<Lhs>::max() > std::numeric_limits<Rhs>::max()), Lhs, Rhs>, Ccy>
        operator -
        (
            monetary_value<Lhs, Ccy> lhs,
            monetary_value<Rhs, Ccy> rhs
        ) noexcept
        {
            auto res {up_cast<Rhs>(lhs)};
            res -= up_cast<Lhs>(rhs);
            return res;
        }
        
        template <numeric_concept Lhs, numeric_concept Rhs, currency Ccy>
        requires (!std::same_as<Lhs, Rhs>)
        constexpr monetary_value<std::conditional_t<(std::numeric_limits<Lhs>::max() > std::numeric_limits<Rhs>::max()), Lhs, Rhs>, Ccy>
        operator *
        (
            monetary_value<Lhs, Ccy> lhs,
            monetary_value<Rhs, Ccy> rhs
        ) noexcept
        {
            auto res {up_cast<Rhs>(lhs)};
            res *= up_cast<Lhs>(rhs);
            return res;
        }
        
        template <numeric_concept Lhs, numeric_concept Rhs, currency Ccy>
        requires (!std::same_as<Lhs, Rhs>)
        constexpr monetary_value<std::conditional_t<(std::numeric_limits<Lhs>::max() > std::numeric_limits<Rhs>::max()), Lhs, Rhs>, Ccy>
        operator /
        (
            monetary_value<Lhs, Ccy> lhs,
            monetary_value<Rhs, Ccy> rhs
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
        lime::monetary_value_concept auto source
    )
    {
        using lime::byte_swap;
        return decltype(source){byte_swap(source.get())};
    }


    //=========================================================================
    [[maybe_unused]]
    static std::string to_string
    (
        lime::monetary_value_concept auto source
    )
    {
        std::string res;
        res += lime::to_string(source.get());
        res += ' ';
        res += lime::to_string(source.ccy);
        return res;
    }

}//!namespace lime


//=========================================================================
[[maybe_unused]]
static std::ostream & operator <<
(
    std::ostream & stream,
    lime::monetary_value_concept auto value
)
{
    stream << lime::to_string(value);
    return stream;
}


//=============================================================================
template<lime::numeric_concept T, lime::currency Ccy>
constexpr lime::monetary_value<T, Ccy>::monetary_value
(
    lime::monetary_value_concept auto const & other
):
    value_(other.value_)
{
}


//=============================================================================
template<lime::numeric_concept T, lime::currency Ccy>
constexpr auto lime::monetary_value<T, Ccy>::operator =
(
    lime::monetary_value_concept auto const & other
) -> monetary_value &
{
    value_ = other.value_;
    return *this;
}


//=============================================================================
template<lime::numeric_concept T, lime::currency Ccy>
constexpr lime::monetary_value<T, Ccy>::monetary_value
(
    lime::monetary_value_concept auto && other
):
    value_(other.value_)
{
}


//=============================================================================
template<lime::numeric_concept T, lime::currency Ccy>
constexpr auto lime::monetary_value<T, Ccy>::operator =
(
    lime::monetary_value_concept auto && other
) -> monetary_value &
{
    value_ = other.value_;
    return *this;
}


//=============================================================================
//todo subject to overflow
template<lime::numeric_concept T, lime::currency Ccy>
template<lime::numeric_concept T1, lime::numeric_concept T2>
constexpr lime::monetary_value<T, Ccy>::monetary_value
(
    lime::price<T1, Ccy> price,
    lime::quantity<T2> quantity
)
{
    price *= quantity.get();
    if constexpr (is_dynamic_value_v<value_type> && is_integral_v<typename lime::price<T1, Ccy>::value_type>)
    {
        value_ = value_type(price.get(), value_.get_precision());
    }
    else if constexpr (is_dynamic_value_v<value_type> && is_fixed_value_v<typename lime::price<T1, Ccy>::value_type>)
    {
        value_ = value_type(static_cast<double>(price.get()));
    }
    else
    {
        value_ = value_type(price.get());
    }
}


//=============================================================================
template<lime::numeric_concept T, lime::currency Ccy>
constexpr lime::monetary_value<T, Ccy>::operator bool()
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
template<lime::numeric_concept T, lime::currency Ccy>
template <std::floating_point T_>
constexpr lime::monetary_value<T, Ccy>::operator T_
(
) const
{
    return static_cast<T_>(value_);
}


//=============================================================================
namespace std {
    
    template<lime::monetary_value_concept T>
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
}//!namspace std
