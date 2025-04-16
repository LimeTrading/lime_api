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
    Creation Date:  April 16th, 2025
*/


#pragma once

#include <include/constexpr_string.h>

#include <cstdint>
#include <string>
#include <concepts>


namespace lime::configuration
{

    template <lime::constexpr_string S, typename T, bool R = true>
    struct configuration_setting
    {
    public:

        static auto constexpr key_name = S;
        using value_type = T;
        static auto constexpr required = R;

        configuration_setting() = default;

        configuration_setting
        (
            T const &
        );

        configuration_setting & operator =
        (
            json const &
        );

        configuration_setting & operator =
        (
            T const &
        );

        bool load
        (
            json const &
        );

        void store
        (
            json &
        ) const;

        auto operator [](std::size_t index) requires requires(T x){x[0];} {return value_[index];}
        auto operator [](std::size_t index) const requires requires(T x){x[0];} {return value_[index];}
        auto size() const requires requires(T x){ x.size();} {return value_.size();}
        auto empty() const requires requires(T x){ x.empty();} {return value_.empty();}
        auto begin() requires requires(T x){ x.begin();} {return value_.begin();}
        auto begin() const requires requires(T x){ x.begin();} {return value_.begin();}
        auto end() requires requires(T x){ x.end();} {return value_.end();}
        auto end() const requires requires(T x){ x.end();} {return value_.end();}
        auto front() requires requires(T x){ x.front();} {return value_.front();}
        auto front() const requires requires(T x){ x.front();} {return value_.front();}
        auto back() requires requires(T x){ x.back();} {return value_.back();}
        auto back() const requires requires(T x){ x.back();} {return value_.back();}
        auto pop_back() requires requires(T x){ x.pop_back();} {return value_.pop_back();}

        T & get();

        T const & get() const;

        template <typename T_>
        operator T_() const requires std::convertible_to<T, T_>;

        template <typename T_>
        bool operator == (T_ const & other) const;

        template <typename T_>
        bool operator != (T_ const & other) const;

    //private:

        friend configuration_setting & operator <<
        (
            // from json
            configuration_setting & destination,
            json const & source
        )
        {
            destination.load(source);
            return destination;
        }

        friend json & operator <<
        (
            // to json
            json & destination,
            configuration_setting const & source
        )
        {
            source.store(destination);
            return destination;
        }

        T                    value_;

        static constexpr lime::constexpr_string name_{S};
    };


    template <lime::constexpr_string S, typename T> using required_setting = configuration_setting<S, T, true>;
    template <lime::constexpr_string S, typename T> using optional_setting = configuration_setting<S, T, false>;

    template <lime::constexpr_string S> using required_string_setting = configuration_setting<S, std::string, true>;
    template <lime::constexpr_string S> using optional_string_setting = configuration_setting<S, std::string, false>;

    template <lime::constexpr_string S> using required_uint32_setting = configuration_setting<S, std::uint32_t, true>;
    template <lime::constexpr_string S> using optional_uint32_setting = configuration_setting<S, std::uint32_t, false>;

    template <lime::constexpr_string S> using required_uint64_setting = configuration_setting<S, std::uint64_t, true>;
    template <lime::constexpr_string S> using optional_uint64_setting = configuration_setting<S, std::uint64_t, false>;

    template <lime::constexpr_string S> using required_int32_setting = configuration_setting<S, std::int32_t, true>;
    template <lime::constexpr_string S> using optional_int32_setting = configuration_setting<S, std::int32_t, false>;

    template <lime::constexpr_string S> using required_int64_setting = configuration_setting<S, std::int64_t, true>;
    template <lime::constexpr_string S> using optional_int64_setting = configuration_setting<S, std::int64_t, false>;

    template <lime::constexpr_string S> using required_bool_setting = configuration_setting<S, bool, true>;
    template <lime::constexpr_string S> using optional_bool_setting = configuration_setting<S, bool, false>;

    //=============================================================================
    template <typename ... S>
    void load_settings
    (
        json const &,
        S & ...
    );

    //=============================================================================
    template <typename ... S>
    void store_settings
    (
        json &,
        S const & ...
    );

    //=========================================================================
    template <lime::constexpr_string S, typename T, bool required>
    static void to_json
    (
        configuration::json & destination,
        configuration_setting<S, T, required> const & source
    )
    {
        source.store(destination);
    }


} // namespace lime::configuration


//=============================================================================
template <typename ... S>
void lime::configuration::load_settings
(
    json const & source,
    S & ... settings
)
{
    (settings.load(source), ...);
}


//=============================================================================
template <typename ... S>
void lime::configuration::store_settings
(
    json & source,
    S const & ... settings
)
{
    (settings.store(source), ...);
}


//=============================================================================
template <lime::constexpr_string S, typename T, bool required>
lime::configuration::configuration_setting<S, T, required>::configuration_setting
(
    T const & value
):
    value_(value)
{
}


//=============================================================================
template <lime::constexpr_string S, typename T, bool required>
auto lime::configuration::configuration_setting<S, T, required>::operator =
(
    T const & value
) -> configuration_setting &
{
    value_ = value;
    return *this;
}


//=============================================================================
template <lime::constexpr_string S, typename T, bool required>
template <typename T_>
bool lime::configuration::configuration_setting<S, T, required>::operator == 
(
    T_ const & other
) const
{
    return (value_ == other);
}


//=============================================================================
template <lime::constexpr_string S, typename T, bool required>
template <typename T_>
bool lime::configuration::configuration_setting<S, T, required>::operator != 
(
    T_ const & other
) const
{
    return (value_ != other);
}


//=============================================================================
template <lime::constexpr_string S, typename T, bool required>
void lime::configuration::configuration_setting<S, T, required>::store
(
    json & destination
) const
{
    destination[S.get()] = value_;
}


//=============================================================================
template <lime::constexpr_string S, typename T, bool required>
bool lime::configuration::configuration_setting<S, T, required>::load
(
    json const & source
)
{
    static auto constexpr key_name = S.get();
    if (source.contains(key_name))
    {
        value_ = source.at(key_name).template get<T>();
        return true;
    }

    if constexpr (required)
        throw std::runtime_error("required configuration setting not found");
    return false;
}


//=============================================================================
template <lime::constexpr_string S, typename T, bool required>
auto lime::configuration::configuration_setting<S, T, required>::operator =
(
    json const & source
) -> configuration_setting &
{
    load(source);
    return *this;
}


//=============================================================================
template <lime::constexpr_string S, typename T, bool required>
T & lime::configuration::configuration_setting<S, T, required>::get
(
)
{
    return value_;
}


//=============================================================================
template <lime::constexpr_string S, typename T, bool required>
T const & lime::configuration::configuration_setting<S, T, required>::get
(
) const
{
    return value_;
}


//=============================================================================
template <lime::constexpr_string S, typename T, bool required>
template <typename T_>
lime::configuration::configuration_setting<S, T, required>::operator T_
(
) const requires std::convertible_to<T, T_>
{
    return value_;
}