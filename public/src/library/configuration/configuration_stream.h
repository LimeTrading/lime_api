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

#include "./configuration.h"

#include <filesystem>


namespace lime::configuration
{

    class configuration_stream
    {
    public:

        configuration_stream() = default;

        configuration_stream
        (
            std::filesystem::path
        );

        configuration_stream & operator = (configuration_stream const &) = default;
        configuration_stream(configuration_stream const &) = default;
        configuration_stream & operator = (configuration_stream &&) = default;
        configuration_stream(configuration_stream &&) = default;

        template <typename T>
        configuration_stream & operator <<
        (
            T const &
        );

        template <typename T>
        configuration_stream & operator >>
        (
            T &
        );

    private:

        friend std::size_t operator <<
        (
            std::filesystem::path,
            configuration_stream const &
        );

        template <typename T>
        friend configuration_stream & operator <<
        (
            T &,
            configuration_stream &
        );

        friend configuration_stream & operator <<
        (
            configuration_stream &,
            std::filesystem::path
        );

        json json_;

    }; // configuration_stream


    template <typename T>
    static auto operator <<
    (
        T & destination,
        configuration_stream & source
    ) -> configuration_stream &
    {
        destination = source.json_.get<T>();
        return source;
    }


} // namespace lime::configuration


//=============================================================================
template <typename T>
auto lime::configuration::configuration_stream::operator <<
(
    // serialize object to json
    T const & object
) -> configuration_stream &
{
    json_ = object;
    return *this;
}


//=============================================================================
template <typename T>
auto lime::configuration::configuration_stream::operator >>
(
    // deserialize json to object
    T & object
) -> configuration_stream &
{
    from_json(json_, object);
    return *this;
}
