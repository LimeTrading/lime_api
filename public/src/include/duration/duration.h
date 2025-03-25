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

#include <concepts>
#include <type_traits>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>

#include <time.h>
#include <sys/time.h>


namespace lime
{

    //=========================================================================
    [[__maybe_unused__]]
    static auto get_time_at_midnight
    (
    )
    {
        ::time_t now = ::time(nullptr);
        struct tm tm;
        localtime_r(&now, &tm);
        tm.tm_sec = 0;
        tm.tm_min = 0;
        tm.tm_hour = 0;
        return std::chrono::system_clock::from_time_t(mktime(&tm)).time_since_epoch();
    }


    //=========================================================================
    [[__maybe_unused__]]
    static auto get_time_since_midnight
    (
    )
    {
        return (std::chrono::system_clock::now().time_since_epoch() - get_time_at_midnight());
    }


    //=========================================================================
    template <typename R, typename P>
    [[__maybe_unused__]]
    std::string to_string
    (
        std::chrono::duration<R, P> source,
        char const * format = "%Y-%m-%d %X"
    )
    {
        std::stringstream s;
        auto time = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>(source));
        s << std::put_time(std::localtime(&time), format);
        return s.str();
    }


    enum class duration_origin : std::uint32_t
    {
        undefined       = 0,
        since_epoch     = 1,
        since_midnight  = 2
    };

    template <typename T>
    concept chrono_duration_concept = std::is_same_v<T, std::chrono::duration<typename T::rep, typename T::period>>;

    template <chrono_duration_concept, duration_origin> 
    class duration;
}


#include "./duration_since_epoch.h"
#include "./duration_since_midnight.h"


namespace lime
{
    
    template <typename T>
    concept duration_concept = std::is_same_v<T, duration<typename T::value_type, T::origin_type>>;


    //=========================================================================
    [[maybe_unused]]
    static inline auto byte_swap
    (
        duration_concept auto source
    ) -> std::decay_t<decltype(source)>
    {
        using lime::byte_swap;
        return std::decay_t<decltype(source)>(byte_swap(source.get()));
    }


    //=========================================================================
    [[maybe_unused]]
    static auto to_underlying
    (
        duration_concept auto const & source
    )
    {
        return source.get();
    }


    //=========================================================================
    [[maybe_unused]]
    static auto operator - 
    (
        duration_concept auto a,
        duration_concept auto b
    ) -> std::decay_t<decltype(a)>
    requires (std::is_same_v<std::decay_t<decltype(a)>, std::decay_t<decltype(b)>>)
    {
        return std::decay_t<decltype(a)>(a.get() - b.get());
    }


    //=========================================================================
    template <typename T0, typename T1>
    [[maybe_unused]] static auto operator - 
    (
        duration_since_epoch<T0> a,
        duration_since_midnight<T1> b
    ) -> duration_since_epoch<T0>
    {
        return duration_since_epoch<T0>(a.get() - b.get());
    }

} // namespace lime
