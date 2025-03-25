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

#include <cstdint>
#include <type_traits>
#include <concepts>
#include <chrono>

#include <byteswap.h>


namespace lime
{

    //==============================================================================
    template <typename T>
    requires ((std::is_integral_v<T>) && (sizeof(T) == 1))
    static constexpr T byte_swap
    (
        T value
    )
    {
        return value;
    }


    //==============================================================================
    template <typename T>
    requires ((std::is_integral_v<T>) && (sizeof(T) == 2))
    static constexpr auto byte_swap
    (
        T value
    )
    {
        auto v = static_cast<std::uint16_t>(value);
        return static_cast<T>((v >> 8) | (v << 8));
    }


    //==============================================================================
    template <typename T>
    requires ((std::is_integral_v<T>) && (sizeof(T) == 4))
    static constexpr auto byte_swap
    (
        T value
    )
    {
        return static_cast<T>(__builtin_bswap32(static_cast<uint32_t>(value)));
    }


    //==============================================================================
    template <typename T>
    requires ((std::is_integral_v<T>) && (sizeof(T) == 8))
    static constexpr auto byte_swap
    (
        T value
    )
    {
        return static_cast<T>(__builtin_bswap64(static_cast<uint64_t>(value)));
    }


    //==============================================================================
    template <typename T>
    requires std::is_enum_v<T>
    static constexpr auto byte_swap
    (
        T value
    )
    {
        return static_cast<T>(byte_swap(static_cast<std::underlying_type_t<T>>(value)));
    }
	

    //=========================================================================
    template <typename rep, typename per>
    static auto byte_swap
    (
        std::chrono::duration<rep, per> source
    ) -> std::chrono::duration<rep, per>
    {
        using lime::byte_swap;
        return std::chrono::duration<rep, per>(byte_swap(source.count()));
    }

    
} // namespace lime
