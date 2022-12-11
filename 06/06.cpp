#include <string>
#include <fstream>
#include <iostream>
#include <deque>
#include <stack>
#include <string_view>
#include <set>

std::string process_input(std::ifstream& f) {
    std::string s;
    f >> s;
    return s;
}

bool is_unique(const std::deque<char>& buffer) {
    std::set<char> set{buffer.begin(), buffer.end()};
    return set.size() == buffer.size();
}

int find_unique(const std::string_view& s, int size) {
    std::deque<char> buffer;
    for (auto it = s.begin(); it != s.end(); ++it) {
        auto c = *it;
        buffer.push_back(c);
        if (buffer.size() == size) {
            if (is_unique(buffer)) return it - s.begin() + 1;
            buffer.pop_front();
        }
    }
    return -1;
}

int p1(const std::string_view& s) {
    return find_unique(s, 4);
}

int p2(const std::string_view& s) {
    return find_unique(s, 14);
}


int main() {
    std::ifstream f{"../06/input"};
    auto input = process_input(f);
    std::cout << p1(input) << std::endl;
    std::cout << p2(input) << std::endl;
}
