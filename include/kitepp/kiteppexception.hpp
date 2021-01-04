/*
 *   Copyright (c) 2020 Bhumit Attarde

 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <exception>
#include <string>
#include <utility>

namespace kitepp
{

using std::string;

/**
 * @brief kitepp saves you the hassle of detecting API errors by looking at HTTP codes or JSON error responses. Instead,
 * it raises aptly named exceptions that you can catch. `kiteppException` is derived from `std::exception` and, along
 * with other useful methods like `code()`, `message()`, provides `what()` method. You should catch exceptions of this
 * class. All kitepp exceptions barring `libException` are derived from this class. `kiteppException` being a abstract
 * class, you cannot create it direectly. Instead, use exceptions provided in `kiteexceptions.hpp` file.
 * @attention libExcpetion isn't derived from this class. You'll either have to catch it directly or catch
 * `std::exception`.
 *
 * @paragraph ex1 example
 * @snippet example2.cpp dealing with kitepp exceptions
 */
class kiteppException : public std::exception
{

  public:
    // constructors and destructor

    kiteppException(int c, string msg) : _code(c), _message(std::move(msg)){};

    // methods

    /**
     * @brief classic `what()` method inherited from `std::exception`
     *
     * @return const char*
     */
    const char *what() const noexcept override = 0;

    /**
     * @brief returns HTTP code sent by REST API
     *
     * @return int
     */
    int code() const noexcept
    {
        return _code;
    };

    /**
     * @brief returns message sent by REST API
     *
     * @return const char*
     */
    const char *message() const noexcept
    {
        return _message.c_str();
    };

  private:
    int _code = 0;
    string _message = "";
};

} // namespace kitepp