#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <cmath>

struct Vec {
    Vec(int x, int y): x(x), y(y) {}
    Vec& operator+=(const Vec& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    float norm() const {
        return std::sqrt(x*x + y*y);
    }

    Vec roundup_unit() const {
        auto norm = this->norm();
        // round up, aka ceil or floor depending on sign
        return Vec((x > 0) ? std::ceil(x/norm) : std::floor(x/norm), 
                   (y > 0) ? std::ceil(y/norm) : std::floor(y/norm));
    }
    int x;
    int y;
};

Vec operator-(const Vec& a, const Vec& b) {
    return Vec(a.x - b.x, a.y - b.y);
}


std::vector<Vec> process_input(std::ifstream& f) {
    std::string row;
    std::vector<Vec> movements;

    std::stringstream ss;
    int n;
    char c;

    std::map<char, Vec> moves{ {'D', {0, -1}}, {'U', {0, 1}}, {'L', {-1, 0}}, {'R', {1, 0}} };

    while (std::getline(f, row)) {
        ss << row;
        ss >> c >> n;
        ss.clear();
        for (int i = 0; i < n; ++i) {
            movements.push_back(moves.at(c));
        }
    }

    return movements;
}

void move_tail(const Vec& H, Vec& T) {
    auto D = H - T;
    if (D.norm() <= std::sqrt(2)+1e-2) { // touching
        return;
    }
    D = D.roundup_unit();
    T += D;
}

uint p1(const std::vector<Vec>& movements) {
    Vec H{0, 0};
    Vec T{0, 0};

    std::unordered_set<std::string> visited;

    for (auto& movement : movements) {
        H += movement;
        move_tail(H, T);

        std::stringstream key;
        key << T.x << "|" << T.y;
        visited.insert(key.str());
    }
    return visited.size();
}

uint p2(const std::vector<Vec>& movements) {
    Vec H{0, 0};
    std::vector<Vec> tails{9, {0, 0}};

    std::unordered_set<std::string> visited;

    for (auto& movement : movements) {
        H += movement;
        for (int i = 0; i < tails.size(); ++i) {
            if (i == 0) move_tail(H, tails[i]);
            else move_tail(tails[i-1], tails[i]);
        }
        std::stringstream key;
        key << tails.back().x << "|" << tails.back().y;
        visited.insert(key.str());
    }
    return visited.size();
}

int main() {
    std::ifstream f{"input"};
    auto movements = process_input(f);
    std::cout << p1(movements) << std::endl;
    std::cout << p2(movements) << std::endl;
}
