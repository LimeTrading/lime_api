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
    Author: SAN
    Creation Date:  March 6, 2025
*/

#pragma once

#include <concepts>
#include <functional>
#include <cstddef>
#include <utility>

namespace lime::alignment {

	template <typename T>
	requires (requires{std::greater<T>{};})
	constexpr T constexpr_max(T const& a, T const& b) noexcept {
		return a > b ? a : b;
	}

	template<typename... Args>
	constexpr auto max = []<std::size_t ... N>(std::index_sequence<N ...> const &) noexcept {
		std::size_t res {0};
		((res = constexpr_max(res, alignof(std::tuple_element_t<N, std::tuple<Args...>>))),  ...);
		return res;
	}(std::make_index_sequence<std::tuple_size_v<std::tuple<Args...>>>());

}//!namespace lime::alignment
