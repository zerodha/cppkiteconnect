#pragma once

#include <string>

//! These defines will dilute the global namespace

#define FMT_HEADER_ONLY 1
#include "fmt/format.h"

using std::string;
using fmt::literals::operator""_a;

#define FMT fmt::format

namespace kitepp::config{

//constants and other things that are needed in more than one file


}