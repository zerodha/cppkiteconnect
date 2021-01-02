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

// headers/macros/operators required in more than one file
//********

//! These defines will dilute the global namespace
#define FMT_HEADER_ONLY 1
#include "fmt/format.h"

using fmt::literals::operator""_a;
#define FMT fmt::format

//*********

namespace kitepp {

// constatnts required in more than one class

constexpr int DEFAULTINT = std::numeric_limits<int>::quiet_NaN();
constexpr double DEFAULTDOUBLE = std::numeric_limits<double>::quiet_NaN();
bool isValid(int num) { return std::isnan(num); };
bool isValid(double num) { return std::isnan(num); };

namespace config {


// configuration related constants and functions *required in more than one file* should go here (e.g., root url, port num etc.)


} // namespace config


} // namespace kitepp