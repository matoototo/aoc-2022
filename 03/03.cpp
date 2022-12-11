#include <iostream>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <numeric>
#include <set>

std::unordered_map<char, int> value_map;

std::vector<std::string> process_input(std::ifstream& f) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(f, line)) {
        lines.push_back(line);
    }
    return lines;
}

int p1(const std::vector<std::string>& lines) {
    int sum = 0;

    for (auto& line : lines) {
        std::string_view sv{line};
        int l_2{static_cast<int>(line.length() / 2)};
        auto first{sv.substr(0, l_2)};
        auto second{sv.substr(l_2)};
        std::set<char> first_set(first.begin(), first.end());
        std::set<char> second_set(second.begin(), second.end());
        std::set<char> inter;
        std::set_intersection(first_set.begin(), first_set.end(), second_set.begin(), second_set.end(), inserter(inter, inter.end()));
        sum += accumulate(inter.begin(), inter.end(), 0, [&](int sum, char c) { return sum + value_map[c]; });
    }

    return sum;
}

int p2(const std::vector<std::string>& lines) {
    int sum = 0;

    for (int i = 0; i < lines.size(); i += 3) {
        std::set<char> l1_set(lines[i].begin(), lines[i].end());
        std::set<char> l2_set(lines[i + 1].begin(), lines[i + 1].end());
        std::set<char> l3_set(lines[i + 2].begin(), lines[i + 2].end());
        std::set<char> inter_, inter;
        std::set_intersection(l1_set.begin(), l1_set.end(), l2_set.begin(), l2_set.end(), inserter(inter_, inter_.end()));
        std::set_intersection(inter_.begin(), inter_.end(), l3_set.begin(), l3_set.end(), inserter(inter, inter.end()));
        sum += accumulate(inter.begin(), inter.end(), 0, [&](int sum, char c) { return sum + value_map[c]; });
    }

    return sum;
}

int main() {
    std::ifstream f("../03/input");

    auto lines = process_input(f);
    for (char c = 'a'; c <= 'z'; c++) {
        auto c_upper{std::toupper(c)};
        value_map[c] = c - 'a' + 1;
        value_map[c_upper] = c_upper - 'A' + 1 + 26;
    }

    std::cout << p1(lines) << std::endl;
    std::cout << p2(lines) << std::endl;
}