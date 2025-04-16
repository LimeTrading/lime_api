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

#include <nlohmann/json.hpp>

#include <filesystem>
#include <chrono>


namespace lime::configuration
{

    using json = nlohmann::json;

    json read_from_file
    (
        std::filesystem::path
    );

    std::size_t write_to_file
    (
        std::filesystem::path,
        json const &
    );

} // namespace lime::configuration


namespace nlohmann {
    template <typename Rep, typename Per>
    struct adl_serializer<std::chrono::duration<Rep, Per>>
    {
        static void to_json(json & destination, std::chrono::duration<Rep, Per> const & source)
        {
            destination = source.count();
        }

        static void from_json(json const & source, std::chrono::duration<Rep, Per> & destination)
        {
            destination = std::chrono::duration<Rep, Per>(source);
        }
    };
}


#include "./configuration_setting.h"
#include "./configuration_stream.h"
