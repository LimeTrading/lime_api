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

#include "./concepts/numeric_concept.h"

#include <string>
#include <string_view>
#include <array>

#include <optional>
#include <charconv>
#include <system_error>

namespace lime
{

	template<numeric_concept Number>
	std::optional<Number> from_string(std::string_view str) noexcept {
		Number local_result;
		auto const last {str.data() + str.size()};

		auto [ptr, ec]{std::from_chars(str.data(), last, local_result)};

		if (ec == std::errc::invalid_argument ||
		    ec == std::errc::result_out_of_range ||
		    ptr != last) {
			return std::nullopt;
		}
		return {local_result};
	}

	template <numeric_concept Number>
	std::string to_string(Number value) noexcept {
		std::array<char, 64> symbols;
		auto [p, ec] = std::to_chars(symbols.data(), symbols.data() + symbols.size(), value);
		return std::string(symbols.data(), p);
	}

} // namespace lime

