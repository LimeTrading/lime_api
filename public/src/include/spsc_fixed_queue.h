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

#include "./non_copyable.h"

#include <concepts>
#include <cstddef>
#include <vector>


namespace lime
{

    template <typename T>
    class spsc_fixed_queue : non_copyable
    {
    public:

        using type = T;
        using value_type = T;

        spsc_fixed_queue
        (
            std::size_t
        );

        spsc_fixed_queue(spsc_fixed_queue &&) = default;
        spsc_fixed_queue & operator = (spsc_fixed_queue &&) = default;
        ~spsc_fixed_queue() = default;

        type pop();

        std::size_t pop
        (
            type &
        );

        std::size_t try_pop
        (
            type &
        );

        template <typename T_>
        bool push
        (
            T_ &&
        );

        template <typename ... Ts>
        bool emplace
        (
            Ts && ...
        );

        T const & front() const;
        
        T & front();

        bool empty() const;

        std::size_t capacity() const;

        std::size_t size() const;

        std::size_t discard();

    private:

        std::size_t volatile        front_;

        std::size_t volatile        back_;

        std::size_t                 capacity_;
        std::size_t                 capacityMask_;

        std::vector<type>           queue_;
    };

} // namespace lime


//==============================================================================
template <typename T>
lime::spsc_fixed_queue<T>::spsc_fixed_queue
(
    std::size_t capacity
):
    front_(0), 
    back_(0)
{
    capacity_ = 1;
    while (capacity_ < capacity)
        capacity_ <<= 1;
    capacityMask_ = capacity_ - 1;
    queue_.resize(capacity_);
}


//==============================================================================
template <typename T>
inline std::size_t lime::spsc_fixed_queue<T>::capacity
(
) const
{
    return capacity_;
}


//==============================================================================
template <typename T>
T & lime::spsc_fixed_queue<T>::front
(
)
{
    return queue_[front_ & capacityMask_];
}


//==============================================================================
template <typename T>
T const & lime::spsc_fixed_queue<T>::front
(
) const
{
    return queue_[front_ & capacityMask_];
}


//==============================================================================
template <typename T>
inline auto lime::spsc_fixed_queue<T>::pop
(
) -> type
{
    std::size_t front = front_;
    type ret = std::move(queue_[front++ & capacityMask_]);
    front_ = front;
    return ret;
}


//==============================================================================
template <typename T>
inline std::size_t lime::spsc_fixed_queue<T>::discard
(
)
{
    queue_[front_ & capacityMask_] = {};
    front_ = front_ + 1;
    return (back_ - front_);
}


//==============================================================================
template <typename T>
inline std::size_t lime::spsc_fixed_queue<T>::pop
(
    type & value
)
{
    auto front = front_;
    auto size = (back_ - front);
    value = std::move(queue_[front++ & capacityMask_]);
    front_ = front;
    return size;
}


//==============================================================================
template <typename T>
inline std::size_t lime::spsc_fixed_queue<T>::try_pop
(
    type & value
)
{
    if (auto front = front_, size = (back_ - front); size > 0)
    {
        value = std::move(queue_[front++ & capacityMask_]);
        front_ = front;
        return size;
    }
    return 0;
}


//==============================================================================
template <typename T>
template <typename ... Ts>
inline bool lime::spsc_fixed_queue<T>::emplace
(
    Ts && ... args
)
{
    if (std::size_t back = back_; (back - front_) < capacity_)
    {
        queue_[back++ & capacityMask_] = T(std::forward<Ts>(args) ...);
        back_ = back;
        return true;
    }
    return false;
}


//==============================================================================
template <typename T>
template <typename T_>
inline bool lime::spsc_fixed_queue<T>::push
(
    T_ && value
)
{
    if (std::size_t back = back_; (back - front_) < capacity_)
    {
        queue_[back++ & capacityMask_] = std::forward<T_>(value);
        back_ = back;
        return true;
    }
    return false;
}


//==============================================================================
template <typename T>
inline bool lime::spsc_fixed_queue<T>::empty
(
) const
{
    return (back_ == front_);
}


//==============================================================================
template <typename T>
inline std::size_t lime::spsc_fixed_queue<T>::size
(
) const
{
    return (back_ - front_);
}
