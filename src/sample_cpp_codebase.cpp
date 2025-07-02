#include "sample_cpp_codebase.hpp"
#include <algorithm>
#include <cctype>

std::string sample_cpp_codebase::reverse(const std::string& str) {
    std::string result = str;
    std::reverse(result.begin(), result.end());
    return result;
}

bool sample_cpp_codebase::isPalindrome(const std::string& str) {
    std::string clean;
    for (char c : str) {
        if (std::isalnum(c)) clean += std::tolower(c);
    }
    return clean == reverse(clean);
}

std::vector<std::string> sample_cpp_codebase::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    for (char c : str) {
        if (c == delimiter) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += c;
        }
    }
    tokens.push_back(token);
    return tokens;
}

std::string sample_cpp_codebase::toUpper(const std::string& str) {
    std::string result;
    for (char c : str) {
        result += std::toupper(c);
    }
    return result;
}

std::string sample_cpp_codebase::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

int sample_cpp_codebase::wordCount(const std::string& str) {
    auto tokens = split(trim(str), ' ');
    int count = 0;
    for (const auto& token : tokens) {
        if (!token.empty()) ++count;
    }
    return count;
}