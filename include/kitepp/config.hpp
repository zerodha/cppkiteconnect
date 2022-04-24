/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2021 Bhumit Attarde
 *
 *  Permission is hereby  granted, free of charge, to any  person obtaining a copy
 *  of this software and associated  documentation files (the "Software"), to deal
 *  in the Software  without restriction, including without  limitation the rights
 *  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
 *  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
 *  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
 *  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
 *  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */
#pragma once

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
#if defined(_WIN32)
// Do nothing (Assuming all modern Windows machines are little endian)
#else // Windows check
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
#endif // Windows check

//!************************************************
// macros/operators required in more than one file*
//!************************************************

using fmt::literals::operator""_a;
#define FMT fmt::format

namespace kiteconnect {

//!*****************************************************************************************************************
// constatnts required in more than one class (other than the explicit user constants defined in uderconstants.hpp)*
//!*****************************************************************************************************************

enum class _methods { GET, POST, PUT, DEL, HEAD };

constexpr int DEFAULTINT = std::numeric_limits<int>::quiet_NaN();
constexpr double DEFAULTDOUBLE = std::numeric_limits<double>::quiet_NaN();
inline bool isValid(int num) { return num == DEFAULTINT; };
inline bool isValid(double num) { return num == DEFAULTDOUBLE; };

namespace config {

//!********************************************************************************************************************
// configuration related constants and functions *required in more than one file* should go here (e.g., root url, port*
// num etc.)
//!********************************************************************************************************************

} // namespace config

} // namespace kiteconnect