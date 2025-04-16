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
#include <library/configuration.h>

#include <cstdint>
#include <string>
#include <iostream>
#include <charconv>


namespace lime::network
{

    class port_id
    {
    public:

        using value_type = std::uint16_t;
        static value_type constexpr any{0};

        port_id() noexcept = default;
        port_id(port_id const &) noexcept = default;
        port_id & operator = (port_id const &) noexcept = default;
        port_id(port_id &&) noexcept = default;
        port_id & operator = (port_id &&) noexcept = default;

        port_id
        (
            std::string const &
        );

        explicit port_id
        (
            value_type
        ) noexcept;

        value_type get() const noexcept;

        bool is_valid() const noexcept;

        auto operator <=>
        (
            port_id const &
        ) const noexcept;

        bool operator ==
        (
            port_id const &
        ) const noexcept;

    private:

        value_type value_{any};
    };


    //=========================================================================
    [[maybe_unused]]
    static port_id byte_swap
    (
        port_id source
    )
    {
        return port_id(lime::byte_swap(source.get()));
    }

    
    //=========================================================================
    [[__maybe_unused__]]
    static void to_json
    (
        configuration::json & destination,
        port_id const & source
    )
    {
        destination = source.get();
    }


    //=========================================================================
    [[__maybe_unused__]]
    static void from_json
    (
        configuration::json const & source,
        port_id & destination
    )
    {
        destination = port_id(source.get<port_id::value_type>());
    }


    //=========================================================================
    [[maybe_unused]]
    static std::string to_string
    (
        port_id portId
    )
    {
        return std::to_string(portId.get());     
    }


    namespace literals
    {
        [[maybe_unused]]
        static port_id operator""_port(unsigned long long int value){return port_id(value);}
    }

} // namespace lime::network
