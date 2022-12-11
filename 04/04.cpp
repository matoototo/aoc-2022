#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

using range = std::pair<int, int>;

std::vector<range> parse_input(std::ifstream& f) {
    std::vector<range> ranges;
    std::string line;
    while (std::getline(f, line)) {
        std::stringstream ss(line);
        std::string range_str;
        while (std::getline(ss, range_str, ',')) {
            std::stringstream range_ss{range_str};
            int a, b;
            range_ss >> a;
            range_ss.ignore();
            range_ss >> b;
            ranges.emplace_back(a, b);
        }
    }
    return ranges;
}

int p1(const std::vector<range>& ranges) {
    int overlaps{0};
    for (int i{0}; i < ranges.size(); i += 2) {
        auto range_a{ranges[i]};
        auto range_b{ranges[i + 1]};
        if (range_a.first <= range_b.first && range_a.second >= range_b.second) {
            overlaps += 1;
        } else if (range_b.first <= range_a.first && range_b.second >= range_a.second) {
            overlaps += 1;
        }
    }
    return overlaps;
}

int p2(const std::vector<range>& ranges) {
    int overlap_atall{0};
    for (int i{0}; i < ranges.size(); i += 2) {
        auto range_a{ranges[i]};
        auto range_b{ranges[i + 1]};
        if (range_a.first <= range_b.first && range_a.second >= range_b.first) {
            overlap_atall += 1;
        } else if (range_b.first <= range_a.first && range_b.second >= range_a.first) {
            overlap_atall += 1;
        }
    }
    return overlap_atall;
}

int main() {
    std::ifstream f("../04/input");
    auto ranges{parse_input(f)};
    std::cout << p1(ranges) << std::endl;
    std::cout << p2(ranges) << std::endl;
}
