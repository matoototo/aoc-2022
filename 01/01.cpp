#include <vector>
#include <numeric>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <algorithm>


struct Elf {
    std::vector<int> food;
    mutable int sum_{0};
    int sum() const {
        if (this->sum_ == 0) this->sum_ = std::accumulate(food.begin(), food.end(), 0);
        return this->sum_;
    }
};

std::vector<Elf> process_input(std::ifstream& in) {
    std::vector<Elf> elves;
    std::string current;
    elves.emplace_back(Elf{});
    while (std::getline(in, current)) {
        if (current.empty())
            elves.emplace_back(Elf{});
        else {
            int calories{std::stoi(current)};
            elves.back().food.push_back(calories);
        }
    }
    return elves;
}

int max_sum(const std::vector<Elf>& elves) {
    int max = 0;
    for (auto& elf : elves) {
        int sum = elf.sum();
        if (sum > max) max = sum;
    }

    return max;
}

void sort_sums(std::vector<Elf>& elves) {
    std::sort(elves.begin(), elves.end(), [](Elf& e1, Elf& e2) { return e1.sum() > e2.sum(); });
}

int sum_top_n(const std::vector<Elf>& elves, int n) {
    assert(n <= elves.size());
    int sum = 0;
    for (auto it = elves.begin(); it != elves.begin() + n; ++it) {
        sum += it->sum();
    }
    return sum;
}

int main() {
    std::ifstream in("input");
    auto elves = process_input(in);
    std::cout << max_sum(elves) << '\n';
    sort_sums(elves);
    std::cout << sum_top_n(elves, 3) << std::endl;
}
