#pragma once
#include <string>
#include <vector>


template <typename... Args> bool is_or(const std::string& value, Args... args) {
    std::vector<std::string> values = {args...};
    return std::find(values.begin(), values.end(), value) != values.end();
}

// and
template <typename... Args> bool is_and(const std::string& value, Args... args) {
    std::vector<std::string> values = {args...};
    return std::all_of(values.begin(), values.end(), [&](const std::string& ext) { return value == ext; });
}