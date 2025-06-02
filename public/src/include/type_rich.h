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
#include <library/configuration.h>

#include <utility>
#include <cstdint>


namespace lime
{

    template <typename T1, std::integral T2, T2 D = {}, bool invalidatable = false> 
    class type_rich;


    template <typename T>
    concept type_rich_concept = std::is_same_v<T, type_rich<typename T::tag_type, typename T::value_type, T::default_value, T::default_is_invalid>>;


    template <typename T1, std::integral T2, T2 D, bool invalidatable>
    class type_rich
    {
    public:

        using tag_type = T1;
        using value_type = T2;
        static auto constexpr default_value = D;
        static auto constexpr default_is_invalid = invalidatable;

        constexpr type_rich() = default;
        constexpr ~type_rich() = default;
        explicit constexpr type_rich(value_type value):value_(value){}
        constexpr type_rich & operator = (value_type value){value_ = value; return *this;}

        template <std::integral T_>
        constexpr type_rich
        (
            type_rich<T1, T_, D, invalidatable> const & other
        ):
            value_(other.get())
        {
        }

        template <std::integral T_>
        constexpr type_rich & operator = 
        (
            type_rich<T1, T_, D, invalidatable> const & other
        )
        {
            value_ = other.get();
            return *this;
        }

        template <std::integral T_>
        constexpr auto operator <=> 
        (
            type_rich<tag_type, T_, default_value, default_is_invalid> const & other
        ) const
        {
            return (value_ <=> other.get());
        }

        template <std::integral T_>
        constexpr auto operator != 
        (
            type_rich<tag_type, T_, default_value, default_is_invalid> const & other
        ) const
        {
            return (value_ != other.get());
        }

        template <std::integral T_>
        constexpr auto operator == 
        (
            type_rich<tag_type, T_, default_value, default_is_invalid> const & other
        ) const
        {
            return (value_ == other.get());
        }

        constexpr auto get() const{return value_;}

        operator bool() const requires (std::is_same_v<value_type, bool>){return value_;}

        bool is_valid() const requires (invalidatable){return (value_ != default_value);}
        void invalidate() requires (invalidatable){value_ = default_value;}

    private:

        value_type  value_{default_value};
    };




    //=========================================================================
    // validatable type rich - allow the default value to indicate invalid value
    // and enable '.is_valid()' and '.invalidate()' methods for type_rich class.
    template <typename T1, std::integral T2, T2 D = {}>
    using validatable_type_rich = type_rich<T1, T2, D, true>;


    //==============================================================================
    [[__maybe_unused__]]
    static inline constexpr auto byte_swap
    (
        type_rich_concept auto value
    ) -> std::decay_t<decltype(value)>
    {
        using lime::byte_swap;
        return std::decay_t<decltype(value)>(byte_swap(value.get()));
    }


    //=========================================================================
    [[maybe_unused]]
    static inline void to_json
    (
        configuration::json & destination,
        type_rich_concept auto const & source
    )
    {
        destination = source.get();
    }


    //=========================================================================
    [[maybe_unused]]
    static void from_json
    (
        configuration::json const & source,
        type_rich_concept auto & destination
    )
    {
        using namespace lime::configuration;
        using T = std::decay_t<decltype(destination)>;
        destination = T(source.get<typename T::value_type>());
    }

} // namespace lime


//=============================================================================
static inline std::ostream & operator << 
(
    std::ostream & s,
    lime::type_rich_concept auto input 
)
{
    s << input.get();
    return s;
}


//=============================================================================
namespace std
{

    template <lime::type_rich_concept T>
    struct hash<T>
    {
        auto operator()(T source)const{return source.get();}
    };

} // namespace std
