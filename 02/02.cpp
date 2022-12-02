#include <vector>
#include <numeric>
#include <string>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <algorithm>

#include <utility>

using round_vector = std::vector<std::pair<uint32_t, uint32_t>>;

round_vector process_input(std::ifstream& in) {
    round_vector rounds;
    std::string current;
    while (std::getline(in, current)) {
        rounds.emplace_back(std::make_pair(current[0] - 'A', current[2] - 'X'));
    }
    return rounds;
}

int score_p1(const round_vector& rounds) {
    uint32_t score = 0;
    for (auto& [opponent, me] : rounds) {
        score += me + 1;
        if (me == opponent) score += 3;
        else if (me - opponent == -2) score += 6;
        else if (me - opponent == 1) score += 6;
    }
    return score;
}

int score_p2(const round_vector& rounds) {
    uint32_t score = 0;
    for (auto& [opponent, me] : rounds) {
        score += me * 3;
        score += (me + opponent + 2) % 3 + 1;
    }
    return score;
}

int main() {
    std::ifstream in("input");
    auto rounds = process_input(in);
    std::cout << score_p1(rounds) << std::endl;
    std::cout << score_p2(rounds) << std::endl;
}
