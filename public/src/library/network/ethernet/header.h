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

#include <library/network/ip/socket_address.h>
#include <include/endian.h>

#include <cstdint>
#include <array>


namespace lime::network
{

    struct ethernet_header
    {
        std::array<std::int8_t, 6>  destinationMacAddress_;
        std::array<std::int8_t, 6>  sourceMacAddress_;
        big_endian<std::uint16_t>   eitherType_;
    };

    struct ipv4_header
    {
        big_endian<std::uint8_t>    versionIhl_;
        big_endian<std::uint8_t>    dscpEcn_;
        big_endian<std::uint16_t>   totalLength_;
        big_endian<std::uint16_t>   identification_;
        big_endian<std::uint16_t>   flagsFragmentOffset_;
        big_endian<std::uint8_t>    timeToLive_;
        big_endian<std::uint8_t>    protocol_;
        big_endian<std::uint16_t>   headerChecksum_;
        ip_address                  sourceIpAddress_;
        ip_address                  destinationIpAddress_;
    };

    struct udp_header
    {
        big_endian<port_id>         sourcePort_;
        big_endian<port_id>         destinationPort_;
        big_endian<std::uint16_t>   totalLength_;
        big_endian<std::uint16_t>   totalChecksum_;
    };

    #pragma pack(push, 1)
    struct udp_packet_header
    {
        ethernet_header ethernetHeader_;
        ipv4_header     ipv4Header_;
        udp_header      udpHeader_;
    };
    #pragma pack(pop)


} // namespace lime::network

