#pragma once

#include <string>
#include <vector>


namespace kitepp {

using std::string;

std::vector<string> _split(const std::string& text, char sep) {

    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        // tokens.push_back(text.substr(start, end - start));
        tokens.emplace_back(text.substr(start, end - start));

        start = end + 1;
    }
    // tokens.push_back(text.substr(start));
    tokens.emplace_back(text.substr(start));

    return tokens;
};


} // namespace kitepp