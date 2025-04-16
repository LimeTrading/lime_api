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
    Author: MAM
    Creation Date:  February 27, 2025
*/

#pragma once

#include <include/endian.h>

#include <cstdint>
#include <string>
#include <span>
#include <string_view>
#include <iostream>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>



namespace lime::network
{

    class ip_address;
    static ip_address byte_swap(ip_address) noexcept;


    class ip_address
    {
    public:

        using value_type = ::in_addr;

        ip_address() noexcept = default;
        ip_address(ip_address const &) noexcept = default;
        ip_address & operator = (ip_address const &) noexcept = default;
        ip_address(ip_address &&) noexcept = default;
        ip_address & operator = (ip_address &&) noexcept = default;

        ip_address  
        (
            std::span<char const> 
        ) noexcept;

        explicit ip_address  
        (
            std::string
        ) noexcept;

        explicit ip_address
        (
            ::in_addr
        ) noexcept;

        bool is_valid() const noexcept;

        operator ::in_addr() const noexcept;

        bool is_loop_back() const noexcept;

        bool is_broadcast() const noexcept;

        bool is_multicast() const noexcept;

        auto operator ==
        (
            ip_address const &
        ) const;
     
        auto operator <=>
        (
            ip_address const &
        ) const noexcept;

    private:
        friend ip_address byte_swap(ip_address) noexcept;

        value_type value_{.s_addr = INADDR_NONE};
    };


    //=========================================================================
    [[maybe_unused]]
    static ip_address byte_swap
    (
        ip_address source
    ) noexcept
    {
        using lime::byte_swap;
        ::in_addr inAddr = ::in_addr(byte_swap(source.value_.s_addr));
        return ip_address(inAddr);
    }


    //=========================================================================
    [[maybe_unused]]
    static std::string to_string
    (
        ip_address ipAddress
    )
    {
        ::in_addr value = ipAddress;
        auto p = reinterpret_cast<std::uint8_t const *>(&value.s_addr);
        return std::to_string(p[0]) + '.' + std::to_string(p[1]) + '.' +
                std::to_string(p[2]) + '.' + std::to_string(p[3]);      
    }

} // namespace lime::network

#include "./host_name.h"
