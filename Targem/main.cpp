#include <algorithm>
#include <iostream>
#include <vector>

#include "ci_string.hpp"


int main() {
    using namespace targem::literals;


    targem::ci_string hello_1 = "Hello";
    hello_1.append(", ").append("World!");

    auto hello_2 = "hELLO, wORLD!"_cis;

    auto sign = [&] {
        switch (targem::compare(hello_1, hello_2)) {
        case -1: return " < ";
        case  0: return " = ";
        case  1: return " > ";
        default: return " ? "; // Will never be executed
        }
    }();

    std::cout << hello_1 << sign << hello_2 << std::endl << std::endl;


    std::size_t n;
    std::cout << "Number of lines: ";
    std::cin >> n;
    std::cin.ignore();

    std::vector<targem::ci_string> vec;
    vec.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        targem::ci_string str;
        std::cin >> str;

        vec.push_back(std::move(str));
    }

    std::sort(vec.begin(), vec.end(), std::greater<>());

    std::cout << std::endl << "Descending order:" << std::endl;
    for (auto const& str : vec) {
        std::cout << str << std::endl;
    }


    std::cin.get();
    return 0;
}