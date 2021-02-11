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

#include <cmath> //isnan()
#include <string>
#define FMT_HEADER_ONLY 1
#include "fmt/format.h"

// Check endieness of platform
/*
   __BIG_ENDIAN__ and __LITTLE_ENDIAN__ are define in some gcc versions
  only, probably depending on the architecture. Try to use endian.h if
  the gcc way fails - endian.h also doesn not seem to be available on all
  platforms.
*/
#ifdef __BIG_ENDIAN__
#define WORDS_BIGENDIAN 1
#else /* __BIG_ENDIAN__ */
#ifdef __LITTLE_ENDIAN__
#undef WORDS_BIGENDIAN
#else
#ifdef BSD
#include <sys/endian.h>
#else
#include <endian.h>
#endif
#if __BYTE_ORDER == __BIG_ENDIAN
#define WORDS_BIGENDIAN 1
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#undef WORDS_BIGENDIAN
#else
#error "unable to determine endianess!"
#endif /* __BYTE_ORDER */
#endif /* __LITTLE_ENDIAN__ */
#endif /* __BIG_ENDIAN__ */

//!************************************************
// macros/operators required in more than one file*
//!************************************************

using fmt::literals::operator""_a;
#define FMT fmt::format

namespace kiteconnect {

//!*****************************************************************************************************************
// constatnts required in more than one class (other than the explicit user constants defined in uderconstants.hpp)*
//!*****************************************************************************************************************

enum class _methods
{
    GET,
    POST,
    PUT,
    DEL,
    HEAD
};

constexpr int DEFAULTINT = std::numeric_limits<int>::quiet_NaN();
constexpr double DEFAULTDOUBLE = std::numeric_limits<double>::quiet_NaN();
bool isValid(int num) { return std::isnan(num); };
bool isValid(double num) { return std::isnan(num); };

namespace config {

//!********************************************************************************************************************
// configuration related constants and functions *required in more than one file* should go here (e.g., root url, port*
// num etc.)
//!********************************************************************************************************************

} // namespace config

} // namespace kiteconnect