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
#include <include/concepts/numeric_concept.h>

#include <string>
#include <cstdint>
#include <type_traits>
#include <concepts>


namespace lime
{

    template <std::integral T> class shares;

    template <typename T>
    concept shares_concept = std::is_same_v<T, shares<typename T::value_type>>;


    template <std::integral T>
    class shares
    {
    public:

        using value_type = T;

        constexpr shares() = default;

        constexpr shares(shares_concept auto const &);
        constexpr shares & operator = (shares_concept auto const &);
        constexpr shares(shares_concept auto &&);
        constexpr shares & operator = (shares_concept auto &&);

        constexpr explicit shares(value_type const & value):value_(value){}
        constexpr value_type get() const{return value_;}

        constexpr shares operator - (numeric_concept auto n){auto value = value_; return {value - n};}
        constexpr shares & operator -= (numeric_concept auto n){value_ -= n; return *this;}
        constexpr shares operator - (shares_concept auto const & other){auto value = value_; return {value - other.value_};}
        constexpr shares & operator -= (shares_concept auto const & other){value_ -= other.get(); return *this;}

        constexpr shares operator + (numeric_concept auto n){auto value = value_; return {value + n};}
        constexpr shares & operator += (numeric_concept auto n){value_ += n; return *this;}
        constexpr shares operator + (shares_concept auto const & other){auto value = value_; return {value + other.value_};}
        constexpr shares & operator += (shares_concept auto const & other){value_ += other.get(); return *this;}

        constexpr shares operator * (numeric_concept auto n) const {return shares(value_ * n);}
        constexpr shares & operator *= (numeric_concept auto n) {value_ *= n; return *this;}
        constexpr shares operator * (shares_concept auto const & other) const {return {value_ * other.value_};}
        constexpr shares & operator *= (shares_concept auto const & other) {value_ *= other.value_; return *this;}

        constexpr shares operator / (numeric_concept auto n) const {return shares(value_ / n);}
        constexpr shares & operator /= (numeric_concept auto n) {value_ /= n; return *this;}
        constexpr shares operator / (shares_concept auto const & other) const {return {value_ / other.value_};}
        constexpr shares & operator /= (shares_concept auto const & other) {value_ /= other.value_; return *this;}

        constexpr auto operator <=> (shares_concept auto const & other) const{return (get() <=> other.get());}
        constexpr auto operator == (shares_concept auto const &  other) const{return (get() == other.get());}       

    private:

        template <std::integral> friend class shares;

        value_type value_;

    }; // class shares<>



    //=========================================================================
    [[__maybe_unused__]]
    static constexpr inline auto byte_swap
    (
        shares_concept auto source
    ) -> std::decay_t<decltype(source)>
    {
        using lime::byte_swap;
        return std::decay_t<decltype(source)>(byte_swap(source.get()));
    }


    //=========================================================================
    [[__maybe_unused__]]
    static std::string to_string
    (
        shares_concept auto source
    )
    {
        return std::to_string(source.get());
    }

} // namespace lime


//=========================================================================
[[__maybe_unused__]]
static std::ostream & operator << 
(
    std::ostream & stream,
    lime::shares_concept auto const & shares 
)
{
    stream << shares.get();
    return stream;
}


//=============================================================================
template <std::integral T>
constexpr lime::shares<T>::shares
(
    shares_concept auto const & other
):
    value_(other.value_)
{
}


//=============================================================================
template <std::integral T>
constexpr auto lime::shares<T>::operator = 
(
    shares_concept auto const & other
) -> shares &
{
    value_ = other.value_;
    return *this;
}


//=============================================================================
template <std::integral T>
constexpr lime::shares<T>::shares
(
    shares_concept auto && other
):
    value_(other.value_)
{
}


//=============================================================================
template <std::integral T>
constexpr auto lime::shares<T>::operator = 
(
    shares_concept auto && other
) -> shares &
{
    value_ = other.value_;
    return *this;
}
