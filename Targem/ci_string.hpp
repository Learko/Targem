#pragma once

#include <cctype>
#include <string>   // char_traits

#include "basic_string.hpp"


namespace targem {


/*
    Case-Insensitive char traits.
*/
struct ci_char_traits : public std::char_traits<char> {
    static bool eq(char lhs, char rhs) {
        return std::tolower(lhs) == std::tolower(rhs);
    }

    static bool lt(char lhs, char rhs) {
        return std::tolower(lhs) < std::tolower(rhs);
    }

    static int compare(char const* lhs, char const* rhs, std::size_t size) {
        for (std::size_t i = 0; i < size; ++i) {
            char const ll = std::tolower(lhs[i]);
            char const lr = std::tolower(rhs[i]);

            if      (ll < lr) { return -1; }
            else if (ll > lr) { return  1; }
        }

        return 0;
    }

    static char const* find(char const* data, std::size_t size, char c) {
        c = std::tolower(c);

        for (std::size_t i = 0; i < size; ++i) {
            if (std::tolower(data[i]) == c) {
                return &data[i];
            }
        }

        return nullptr;
    }
};


/*
    Case-Insensitive string class.
*/
using ci_string = basic_string<char, ci_char_traits>;


namespace literals {

ci_string operator"" _cis(char const* data, std::size_t size) {
    return ci_string(data, size);
}

} // namespace literals

} // namespace targem