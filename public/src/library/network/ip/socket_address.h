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

#include "./ip_address.h"
#include "./port_id.h"

#include <library/configuration.h>

#include <algorithm>
#include <string>
#include <iostream>
#include <charconv>
#include <string_view>
#include <system_error>


namespace lime::network
{

    #pragma pack(push, 1)
    class socket_address
    {
    public:

        socket_address() noexcept = default;
        socket_address(socket_address const &) noexcept = default;
        socket_address & operator = (socket_address const &) noexcept = default;
        socket_address(socket_address &&) noexcept = default;
        socket_address & operator = (socket_address &&) noexcept = default;

        socket_address
        (
            std::span<char const> 
        );

        socket_address
        (
            std::string const & 
        );

        socket_address
        (
            ip_address,
            port_id
        ) noexcept;

        socket_address
        (
            ip_address
        ) noexcept;

        socket_address
        (
            ::sockaddr_in
        ) noexcept;

        ip_address get_ip_address() const noexcept;

        port_id get_port_id() const noexcept;
        
        operator ::sockaddr_in() const noexcept;

        bool is_valid() const noexcept;

        bool is_multicast() const noexcept;

        auto operator <=> 
        (
            socket_address const &
        ) const noexcept;

        bool operator == 
        (
            socket_address const &
        ) const noexcept;

    private:

        ip_address  ipAddress_{};

        port_id     portId_{};

    }; // class socket_address
    #pragma pack(pop)

    static_assert(sizeof(socket_address) == 6);


    //=========================================================================
    [[maybe_unused]]
    static socket_address byte_swap
    (
        socket_address source
    ) noexcept
    {
        using lime::byte_swap;
        return socket_address(byte_swap(source.get_ip_address()), byte_swap(source.get_port_id()));
    }


    //=========================================================================
    static std::string to_string
    (
        socket_address socketAddress
    )
    {
        return to_string(socketAddress.get_ip_address()) + ':' + to_string(socketAddress.get_port_id());     
    }


    //=========================================================================
    [[maybe_unused]]
    static void to_json
    (
        lime::configuration::json & destination,
        socket_address const & source
    )
    {
        using namespace lime::configuration;
        destination = to_string(source);
    }


    //=========================================================================
    [[maybe_unused]]
    static void from_json
    (
        lime::configuration::json const & source,
        socket_address & destination
    )
    {
        using namespace lime::configuration;
        destination = source.get<std::string>();
    }

} // namespace lime::network
