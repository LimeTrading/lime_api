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


namespace lime
{

    template <chrono_duration_concept T>
    class duration<T, duration_origin::since_epoch>
    {
    public:

        static auto constexpr origin_type = duration_origin::since_epoch;
        using value_type = T;

        duration() = default;

        template <chrono_duration_concept T_>
        duration
        (
            duration<T_, origin_type> const &
        );

        template <chrono_duration_concept T_>
        duration & operator = 
        (
            duration<T_, origin_type> const &
        );

        template <chrono_duration_concept T_>
        duration
        (
            duration<T_, duration_origin::since_midnight> const &
        );

        template <chrono_duration_concept T_>
        duration & operator = 
        (
            duration<T_, duration_origin::since_midnight> const &
        );

        template <chrono_duration_concept T_>
        explicit duration
        (
            T_
        );

        template <chrono_duration_concept T_>
        duration & operator += 
        (
            duration<T_, origin_type>
        );

        template <chrono_duration_concept T_>
        duration operator + 
        (
            duration<T_, origin_type>
        ) const;

        value_type get() const;

        static duration now();
        static duration at_start_of_day();

        auto operator <=> (duration const & other) const{return (value_ <=> other.value_);}
        auto operator == (duration const & other) const{return (value_ == other.value_);}

        std::uint32_t get_day_of_month
        (
        ) const
        {
            using namespace std::chrono;
            auto time = system_clock::to_time_t(time_point<system_clock>(value_));
            return std::localtime(&time)->tm_mday;
        }


        std::uint32_t get_month
        (
        ) const
        {
            using namespace std::chrono;
            auto time = system_clock::to_time_t(time_point<system_clock>(value_));
            return std::localtime(&time)->tm_mon;
        }


        std::uint32_t get_year
        (
        ) const
        {
            using namespace std::chrono;
            auto time = system_clock::to_time_t(time_point<system_clock>(value_));
            return std::localtime(&time)->tm_year;
        }

        explicit operator bool() const {return (value_.count() != 0);}
    protected:

        value_type value_{0};

    }; // class duration<T, since_epoch>

    template <chrono_duration_concept T> using duration_since_epoch = duration<T, duration_origin::since_epoch>;
    template <chrono_duration_concept T> using duration_since_midnight = duration<T, duration_origin::since_midnight>;

    using nanoseconds_since_epoch = duration_since_epoch<std::chrono::nanoseconds>;
    using microseconds_since_epoch = duration_since_epoch<std::chrono::microseconds>;
    using milliseconds_since_epoch = duration_since_epoch<std::chrono::milliseconds>;
    using seconds_since_epoch = duration_since_epoch<std::chrono::seconds>;


    //=========================================================================
    template <chrono_duration_concept T>
    static auto to_string
    (
        duration_since_epoch<T> source,
        char const * format = "%Y-%m-%d %X"
    )
    {
        return to_string(source.get(), format);
    }

} // namespace lime


//=============================================================================
template <lime::chrono_duration_concept T>
template <lime::chrono_duration_concept T_>
lime::duration_since_epoch<T>::duration
(
    // construct from any other duration_since_epoch type
    duration_since_epoch<T_> const & other
):
    value_(std::chrono::duration_cast<T>(other.get()))
{
}


//=============================================================================
template <lime::chrono_duration_concept T>
template <lime::chrono_duration_concept T_>
auto lime::duration_since_epoch<T>::operator = 
(
    // copy construct from any other duration_since_epoch type
    duration_since_epoch<T_> const & other
) -> duration &
{
    value_ = std::chrono::duration_cast<T>(other.get());
    return *this;
}


//=============================================================================
template <lime::chrono_duration_concept T>
template <lime::chrono_duration_concept T_>
lime::duration_since_epoch<T>::duration
(
    // construct from a *duration_since_midnight* type
    duration<T_, duration_origin::since_midnight> const & other
)
{
    auto otherInNs = std::chrono::duration_cast<std::chrono::nanoseconds>(other.get());
    value_ = std::chrono::duration_cast<T>(otherInNs + get_time_at_midnight());
}


//=============================================================================
template <lime::chrono_duration_concept T>
template <lime::chrono_duration_concept T_>
auto lime::duration_since_epoch<T>::operator = 
(
    // copy construct from a *duration_since_midnight* type
    duration<T_, duration_origin::since_midnight> const & other
) -> duration &
{
    auto otherInNs = std::chrono::duration_cast<std::chrono::nanoseconds>(other.get());
    value_ = std::chrono::duration_cast<T>(otherInNs + get_time_at_midnight());
    return *this;
}


//=============================================================================
template <lime::chrono_duration_concept T>
template <lime::chrono_duration_concept T_>
lime::duration_since_epoch<T>::duration
(
    // construct from raw chrono duration
    T_ value
): 
    value_(std::chrono::duration_cast<T>(value))
{
}


//=============================================================================
template <lime::chrono_duration_concept T>
auto lime::duration_since_epoch<T>::get
(
) const -> value_type
{
    return value_;
}


//=============================================================================
template <lime::chrono_duration_concept T>
auto lime::duration_since_epoch<T>::now
(
) -> duration_since_epoch<T>
{
    auto nsSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
    return duration_since_epoch<value_type>(std::chrono::duration_cast<value_type>(nsSinceEpoch));
}


//=============================================================================
template <lime::chrono_duration_concept T>
auto lime::duration_since_epoch<T>::at_start_of_day
(
) -> duration_since_epoch<T>
{
    auto sinceMidnight = duration_since_midnight<value_type>::now();
    return duration_since_epoch<T>(now() - sinceMidnight);
}


//=============================================================================
template <lime::chrono_duration_concept T>
template <lime::chrono_duration_concept T_>
auto lime::duration_since_epoch<T>::operator += 
(
    duration<T_, origin_type> other
) -> duration_since_epoch<T> &
{
    value_ += other.value_;
    return *this;
}


//=============================================================================
template <lime::chrono_duration_concept T>
template <lime::chrono_duration_concept T_>
auto lime::duration_since_epoch<T>::operator + 
(
    duration<T_, origin_type> other
) const -> duration_since_epoch<T>
{
    auto ret = *this;
    ret.value_ += other.get();
    return ret;
}
