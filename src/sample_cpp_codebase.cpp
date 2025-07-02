#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

class StringUtils {
public:
    // Reverses a string
    static std::string reverse(const std::string& str) {
        std::string result = str;
        std::reverse(result.begin(), result.end());
        return result;
    }

    // Checks if a string is a palindrome
    static bool isPalindrome(const std::string& str) {
        std::string clean;
        for (char c : str) {
            if (std::isalnum(c)) clean += std::tolower(c);
        }
        return clean == reverse(clean);
    }

    // Splits a string by a given delimiter
    static std::vector<std::string> split(const std::string& str, char delimiter) {
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

    // Converts a string to uppercase
    static std::string toUpper(const std::string& str) {
        std::string result;
        for (char c : str) {
            result += std::toupper(c);
        }
        return result;
    }

    // Trims whitespace from both ends
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }

    // Counts words in a sentence
    static int wordCount(const std::string& str) {
        auto tokens = split(trim(str), ' ');
        int count = 0;
        for (const auto& token : tokens) {
            if (!token.empty()) ++count;
        }
        return count;
    }
};

int main() {
    std::string input = "   Madam In Eden, I'm Adam   ";
    std::cout << "Original: '" << input << "'\n";
    std::cout << "Reversed: '" << StringUtils::reverse(input) << "'\n";
    std::cout << "Is Palindrome: " << (StringUtils::isPalindrome(input) ? "Yes" : "No") << "\n";
    std::cout << "Uppercase: '" << StringUtils::toUpper(input) << "'\n";
    std::cout << "Trimmed: '" << StringUtils::trim(input) << "'\n";
    std::cout << "Word Count: " << StringUtils::wordCount(input) << "\n";

    return 0;
}
