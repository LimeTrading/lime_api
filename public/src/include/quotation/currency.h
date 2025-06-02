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

/*
 * this follows
 * ISO 4217 currency codes
 * non ISO 4217 currency codes - ie CNH and CNT
 * non ISO 4217 currency codes - ie crypto
 */


#include "include/alignment.h"

#include <cstring>
#include <algorithm>

namespace lime {
    
    enum class currency_type : std::uint8_t {
        undefined               = 255,
        national                = 0,
        crypto                  = 1,
    };
    
    enum class currency_minor_units : std::uint8_t {
        undefined               = 255,
        no_units                = 0,
        one_tenth               = 1,
        one_hundredth           = 2,
        one_thousandth          = 3,
        wei                     = 18,
    };
    
    constexpr std::uint8_t get_decimals_count
    (
        currency_minor_units units
    )
    {
        return lime::pow_10(static_cast<std::uint64_t>(units));
    }
    
    static inline constexpr std::size_t currency_code_sz{3u};
    
    static inline constexpr std::size_t currency_num_sz{3u};
    
    constexpr std::size_t constexpr_strlen
    (
        const char* str
    )
    {
        std::size_t res {0u};
        while (str[res] != '\0') ++res;
        return res;
    }


#pragma pack(push, 1)
    struct
    alignas (std::uint64_t)
    currency final {
        
        std::array<char, currency_code_sz> code_ {};
        std::array<char, currency_num_sz> num_ {}; //there can be leading zeros
        currency_minor_units D_;
        currency_type type_;
        
        constexpr explicit currency
        (
            const char* code,
            const char* num,
            currency_minor_units D,
            currency_type type
        ):
            D_(D),
            type_(type)
        {
            
            std::size_t sz {std::min(constexpr_strlen(code), currency_code_sz)};
            std::copy(code, code + sz, code_.begin());
            
            sz = std::min(constexpr_strlen(num), currency_num_sz);
            std::copy(num, num + sz, num_.begin());
            
        }
        
        constexpr currency(currency const&) = default;
        constexpr currency(currency &&) = default;
        constexpr currency& operator = (currency const&) = default;
        constexpr currency& operator = (currency &&) = default;
        
        constexpr auto operator <=> (currency const& other) const noexcept = default;
        constexpr bool operator == (currency const& other) const noexcept = default;
    };
#pragma pack(pop)
    static_assert(sizeof(currency) == 8u, "Check currency, something is broken with its size");
    
    
    template<typename T>
    concept currency_concept = requires (T t) {
        { t.code_ } -> std::same_as<std::array<char, currency_code_sz>>;
        { t.num_ }  -> std::same_as<std::array<char, currency_num_sz>>;
        { t.D_ }    -> std::same_as<currency_minor_units>;
        { t.type_ } -> std::same_as<currency_type>;
    };
    
    namespace ccy {
        
        static inline constexpr currency
            no_currency ("XXX", "999", currency_minor_units::undefined, currency_type::undefined),
            cny ("CNY", "156", currency_minor_units::one_hundredth, currency_type::national),
            eur ("EUR", "978", currency_minor_units::one_hundredth, currency_type::national),
            usd ("USD", "840", currency_minor_units::one_hundredth, currency_type::national);
        
    }//!namespace ccy
    
    
    // TODO: allow for custom builds with different default currency
    static auto constexpr default_currency = ccy::usd;

    [[maybe_unused]]
    static std::string to_string
    (
        lime::currency source
    )
    {
        std::string res;
        res.reserve(lime::currency_code_sz);
        std::copy(source.code_.cbegin(), source.code_.cend(), std::back_inserter(res));
        
        return res;
    }
    
}//!namespace lime

namespace std {
    
    template<lime::currency_concept T>
    struct hash<T>
    {
        std::size_t operator()
        (
            lime::currency c
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
            
            for (char ch : c.code_) hash_combine(h, ch);
            for (char ch : c.num_)  hash_combine(h, ch);
            hash_combine(h, static_cast<std::underlying_type_t<lime::currency_minor_units>>(c.D_));
            hash_combine(h, static_cast<std::underlying_type_t<lime::currency_type>>(c.type_));
            
            return h;
        }
    };
}//!namespace std