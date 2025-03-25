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

#include <include/non_copyable.h>

#include <span>
#include <type_traits>
#include <cstdint>
#include <functional>
#include <vector>
#include <queue>
#include <concepts>


namespace lime::message
{

    template <typename T, protocol_concept P>
    class receiver :
        virtual non_copyable
    {
    public:

        static auto constexpr allow_polymorphic_target = false;

        using target = std::decay_t<T>;
        using protocol = P;
        using protocol_traits = typename protocol::traits;
        using message_indicator = protocol_traits::message_indicator;
        using underlying_message_indicator = std::make_unsigned_t<std::underlying_type_t<message_indicator>>;

        std::span<char const> process 
        (
            std::span<char const>
        );

    protected:

        receiver();
        receiver(receiver &&) = default;
        receiver & operator = (receiver &&) = default;
        ~receiver() = default;

        void process_message
        (
            std::span<char const> source
        )
        {
            using message_header = lime::message::message_header<P>;
            message_header const & messageHeader = *reinterpret_cast<message_header const *>(source.data());
            if (auto callback = callback_[static_cast<underlying_message_indicator>(messageHeader.get_message_indicator())]; callback != nullptr)
                callback(*this, source.data());
        }

    private:

        static auto constexpr bits_per_byte = 8;
        static auto constexpr max_underlying_message_indicator_value = (1 << (sizeof(underlying_message_indicator) * bits_per_byte));

        template <message_indicator M>
        requires ((not std::is_polymorphic_v<target>) || ((std::is_polymorphic_v<target>) && allow_polymorphic_target))
        static void dispatch_message
        (
            receiver & self,
            void const * address
        )
        {
            using message_type = message<protocol, M>;
            if constexpr (std::is_polymorphic_v<target>)
                dynamic_cast<target &>(self)(*reinterpret_cast<message_type const *>(address)); // careful this path is slow
            else
                reinterpret_cast<target &>(self)(*reinterpret_cast<message_type const *>(address)); // better to ensure that target is not polymorphic
        }

        static std::array<void(*)(receiver &, void const *), max_underlying_message_indicator_value> callback_;

    }; // class receiver


    template <typename T, protocol_concept P>
    std::array<void(*)(receiver<T, P> &, void const *), receiver<T, P>::max_underlying_message_indicator_value> receiver<T, P>::callback_;


    template <typename T>
    concept receiver_concept = std::is_same_v<T, receiver<typename T::target, typename T::protocol>>;

} // namespace lime::message


//=============================================================================
template <typename T, lime::message::protocol_concept P>
lime::message::receiver<T, P>::receiver 
(
)
{

    static_assert(not std::is_polymorphic_v<receiver<T, P>>);

    [[__maybe_unused__]] static auto once = [&]<std::size_t ... N>(std::index_sequence<N ...>)
    {
        for (auto & callback : callback_)
            callback = nullptr;
        ([&]()
            {    
                // only configure a callback if 'target' supports receiving that message type
                if constexpr (requires (target t, message<protocol, P::get(N)> m){t(m);})
                    callback_[static_cast<underlying_message_indicator>(P::get(N))] = dispatch_message<P::get(N)>;
                else
                {
                    // TODO: add some kind of warning that this type of receiver has no handler for this type of message
                }
            }(), ...);
        return true;
    }(std::make_index_sequence<protocol::messageIndicators_.size()>());
}


//=============================================================================
template <typename T, lime::message::protocol_concept P>
auto lime::message::receiver<T, P>::process 
(
    std::span<char const> source
) -> std::span<char const>
{
    using message_header = message_header<protocol>;
    static auto constexpr minimum_data_to_parse_header = sizeof(message_header);

    auto cur = source.data();
    auto bytesRemaining = source.size(); 
    while (bytesRemaining > 0)
    {
        if (bytesRemaining < minimum_data_to_parse_header)
            break;

        // there is sufficient data in the next packet to represent a header
        auto const & messageHeader = *reinterpret_cast<message_header const *>(cur);
        auto messageSize = messageHeader.size();
        if (messageSize < minimum_data_to_parse_header)
        {
            // obvious bad data
            break;
        }
        if (bytesRemaining < messageSize)
            break; // have partial message?

        // the next packet has sufficient data to represent an entire message
        process_message(std::span(reinterpret_cast<char const *>(&messageHeader), messageSize)); // dispatch the message
        cur += messageSize;
        bytesRemaining -= messageSize;
    }
    return {cur, bytesRemaining};
}

