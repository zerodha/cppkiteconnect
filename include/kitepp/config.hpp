#pragma once

#include <string>

//! These defines will dilute the global namespace
#define FMT_HEADER_ONLY 1
#include "fmt/format.h"

using std::string;
using fmt::literals::operator""_a;

#define FMT fmt::format

namespace kitepp {

namespace config {


// constants and other things thatare needed in more than one file


}


} // namespace kitepp