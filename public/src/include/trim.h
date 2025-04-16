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
    Creation Date:  April 16, 2025
*/


#pragma once

#include <string_view>
#include <span>


namespace lime
{
    [[__maybe_unused__]]
    static std::string_view const trim
    (
        std::span<char const> source
    )
    {
        static auto constexpr is_whitespace = [](auto c){for (auto w : {'\t', '\r', '\n', ' ', '\0'}) if (c == w) return true; return false;};
        auto begin = source.data();
        auto end = begin + source.size();
        while ((begin < end) && (is_whitespace(*begin)))
            ++begin;
        while ((end > begin) && (is_whitespace(end[-1])))
            --end;
        return {begin, (unsigned)std::distance(begin, end)};
    } 

}