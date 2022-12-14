#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <memory>
#include <optional>
#include <set>
#include <sstream>

using Tile = std::pair<int, int>;
using Grid = std::set<Tile>;

std::set<Tile> range(const std::pair<int, int>& from, const std::pair<int, int>& to) {
    std::set<Tile> tiles;
    auto [x1, y1] = from;
    auto [x2, y2] = to;
    if (x1 == x2 && y1 != y2) {
        auto lower = std::min(y1, y2);
        auto upper = std::max(y1, y2);
        for (auto y = lower; y <= upper; ++y) {
            tiles.insert({x1, y});
        }
    }
    else if (x1 != x2 && y1 == y2) {
        auto lower = std::min(x1, x2);
        auto upper = std::max(x1, x2);
        for (auto x = lower; x <= upper; ++x) {
            tiles.insert({x, y1});
        }
    }
    return tiles;
}

void add_to_grid(Grid& grid, std::vector<std::pair<int, int>> pairs) {
    for (auto it = pairs.begin(); it != pairs.end() - 1; ++it) {
        auto new_tiles = range(*it, *(it+1));
        grid.insert(new_tiles.begin(), new_tiles.end());
    }
}

Grid process_input(std::ifstream& f) {
    Grid grid;
    std::string line, arrow;
    int x, y;
    char _;
    while (std::getline(f, line)) {
        std::vector<std::pair<int, int>> pairs;
        bool first = true;
        std::stringstream ss;
        ss << line;
        while (ss) {
            if (first) first = false;
            else ss >> arrow;
            ss >> x >> _ >> y >> _;
            pairs.emplace_back(x, y);
        }
        add_to_grid(grid, pairs);
    }
    return grid;
}

int lower_bound(const Grid& grid) {
    int y_max{0};
    for (auto& [x, y] : grid) {
        y_max = std::max(y, y_max);
    }
    return y_max;
}

bool compute_fall(const Grid& grid, Tile& current, int floor = 1e9) {
    auto& [x, y] = current;
    auto y_clamp = std::min(y+1, floor-1);

    Tile down{x, y_clamp};
    Tile downleft{x-1, y_clamp};
    Tile downright{x+1, y_clamp};

    if (!grid.contains(down)) {
        current = down;
        return true;
    } else if (!grid.contains(downleft)) {
        current = downleft;
        return true;
    } else if (!grid.contains(downright)) {
        current = downright;
        return true;
    }

    return false;
}

int p1(Grid grid) {
    auto lowest = lower_bound(grid);
    int count = 0;
    while (true) {
        Tile current{500, 0};
        while (compute_fall(grid, current) && lowest >= current.second);
        if (current.second > lowest) break;
        grid.insert(current);
        ++count;
    }
    return count;
}

int p2(Grid grid) {
    auto floor = lower_bound(grid) + 2;
    int count = 0;
    while (true) {
        Tile current{500, 0};
        while (compute_fall(grid, current, floor) && floor - 1 > current.second);
        ++count;
        grid.insert(current);
        if (current == Tile{500, 0}) break;
    }
    return count;
}


int main() {
    std::ifstream f{"../14/input"};
    auto grid = process_input(f);
    std::cout << p1(grid) << std::endl;
    std::cout << p2(grid) << std::endl;
}
