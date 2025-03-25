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

#include "./protocol_traits.h"

#include <tuple>
#include <array>


namespace lime::message
{

    template <protocol_traits_concept T0, typename T0::message_indicator ... T1>
    struct protocol
    {
        using traits = T0;
        using message_indicator = typename T0::message_indicator;
        static auto constexpr message_arity = sizeof ... (T1);

        static constexpr std::array<message_indicator, message_arity> messageIndicators_{T1 ...};
        static constexpr message_indicator get(std::size_t index){return messageIndicators_[index];}
    };


    template <typename T>
    concept protocol_concept = std::is_same_v<T, 
            decltype([]<std::size_t ... N>(std::index_sequence<N ...>) -> protocol<typename T::traits, T::get(N) ...>
            {return {};}(std::make_index_sequence<T::messageIndicators_.size()>()))>;


} // namespace lime::message


//=============================================================================
static constexpr auto operator <=>
(
    lime::message::protocol_concept auto const & first,
    lime::message::protocol_concept auto const & second
) noexcept
{
    if constexpr (auto a = (first.traits() <=> second.traits()); a != 0)
        return a;
    return (first.messagesIndicators_ <=> second.messagesIndicators_);
}


//=============================================================================
static constexpr auto operator ==
(
    lime::message::protocol_concept auto const & first,
    lime::message::protocol_concept auto const & second
) noexcept
{
    if constexpr (auto a = (first.traits() == second.traits()); a != 0)
        return a;
    return (first.messagesIndicators_ == second.messagesIndicators_);
}
