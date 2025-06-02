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

#include "./monetary_value.h"

namespace lime {
        
        template <numeric_concept T0, currency Ccy, numeric_concept T1>
        class notional_value final {
        public:
            
            static constexpr currency ccy = Ccy;
            
            using value_type = std::decay_t<decltype(get_max_numeric_limit<T0, T1>())>::type;
            
            constexpr notional_value() = default;
            
            constexpr notional_value(notional_value const &) = default;
            
            notional_value &operator=(notional_value const &) = default;
            
            constexpr notional_value(notional_value &&) = default;
            
            notional_value &operator=(notional_value &&) = default;
            
            constexpr explicit notional_value (price_concept auto const&, quantity_concept auto const&);
            
            constexpr explicit operator bool() const noexcept;
            
            [[nodiscard]] operator monetary_value<value_type, ccy> () const noexcept;

            auto get() const noexcept { return static_cast<monetary_value<value_type, ccy>>(*this); }
            
            constexpr auto operator<=>(notional_value const& other) const noexcept = default;
            constexpr bool operator==(notional_value const& other) const noexcept = default;
            constexpr bool operator!=(notional_value const& other) const noexcept = default;

        private:
            
            price<T0, Ccy> price_;
            quantity<T1> quantity_;

        }; // class notional_value<>

        template<typename T>
        concept notional_value_concept = requires(T t) {
            typename T::value_type;
            { T::ccy } -> std::same_as<currency>;
            { static_cast<monetary_value<typename T::value_type, T::ccy>>(t) };
        };
        
    //=========================================================================
    [[maybe_unused]]
    static constexpr inline auto byte_swap
    (
        lime::notional_value_concept auto source
    )
    {
        using lime::byte_swap;
        return decltype(source){byte_swap(source.get())};
    }


    //=========================================================================
    [[maybe_unused]]
    static std::string to_string
    (
        lime::notional_value_concept auto source
    )
    {
        std::string res;
        res += lime::to_string(source.get());
        return res;
    }

}//!namespace lime


//=========================================================================
[[maybe_unused]]
static std::ostream & operator <<
(
    std::ostream & stream,
    lime::notional_value_concept auto notional_value
)
{
    stream << lime::to_string(notional_value);
    return stream;
}


//=============================================================================
template<lime::numeric_concept T0, lime::currency Ccy, lime::numeric_concept T1>
constexpr lime::notional_value<T0, Ccy, T1>::notional_value
(
    lime::price_concept auto const& price,
    quantity_concept auto const& quantity
) :
price_(price),
quantity_(quantity)
{
}

//=============================================================================
template<lime::numeric_concept T0, lime::currency Ccy, lime::numeric_concept T1>
constexpr lime::notional_value<T0, Ccy, T1>::operator bool()
const noexcept
{
    return price_.operator bool() && quantity_.operator bool();
}

//=============================================================================
template<lime::numeric_concept T0, lime::currency Ccy, lime::numeric_concept T1>
lime::notional_value<T0, Ccy, T1>::operator monetary_value<value_type, ccy> () const noexcept{
    return monetary_value<value_type, ccy>(price_, quantity_);
}


//=============================================================================
namespace std {
    
    template<lime::notional_value_concept T>
    struct hash<T>
    {
        auto operator()
        (
            T source
        ) const noexcept
        {
            auto const _{source.get()};
            return std::hash<std::decay_t<decltype(_)>>{}(_);
        }
    };
}
