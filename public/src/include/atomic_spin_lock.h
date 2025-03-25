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

#include <thread>
#include <atomic>
#include <cstdint>


namespace lime
{

    class atomic_spin_lock final :
        non_copyable
    {
    public:

        atomic_spin_lock() = default;
        ~atomic_spin_lock() = default;

        atomic_spin_lock(atomic_spin_lock &&) = default;
        atomic_spin_lock & operator = (atomic_spin_lock &&) = default;

        void lock();

        void unlock();

        bool try_lock();

    private:

        std::atomic<std::thread::id> value_{};

    }; // atomic_spin_lock

} // namespace lime


//=============================================================================
inline void lime::atomic_spin_lock::lock
(
)
{
    std::thread::id expected = {};
    while (not value_.compare_exchange_strong(expected, std::this_thread::get_id()))
        ;
}


//=============================================================================
inline void lime::atomic_spin_lock::unlock
(
)
{
    auto expected = std::this_thread::get_id();
    value_.compare_exchange_strong(expected, {});
}


//=============================================================================
inline bool lime::atomic_spin_lock::try_lock
(
)
{
    std::thread::id expected = {};
    return value_.compare_exchange_strong(expected, std::this_thread::get_id());
}
