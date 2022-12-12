#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
#include <functional>
#include <string>
#include <sstream>
#include <memory>
#include <queue>
#include <set>
#include <unordered_set>

struct Node {
    Node(int h, int x, int y): height(h), x(x), y(y) {}
    int height, x, y;
    std::vector<std::shared_ptr<Node>> neighbours;
    int distance = 0;
    int to_end = 0;
};

void set_neighbours(std::vector<std::shared_ptr<Node>>& nodes, std::shared_ptr<Node> node, const size_t row_length) {
    if (nodes.size() > row_length) {
        auto& up = nodes[nodes.size() - row_length - 1];
        up->neighbours.push_back(node);
        node->neighbours.push_back(up);
    }
    if (nodes.size() % row_length != 1) {
        auto& left = nodes[nodes.size() - 2];
        left->neighbours.push_back(node);
        node->neighbours.push_back(left);
    }
}

int get_height(char c) {
    if (c == 'S') return 0;
    else if (c == 'E') return 'z' - 'a';
    return c - 'a';
}

std::tuple<std::shared_ptr<Node>, std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>> process_input(std::ifstream& f) {
    std::string row;
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Node>> lowest_nodes;
    std::shared_ptr<Node> start = nullptr, end = nullptr;
    int j = 0;
    while (std::getline(f, row)) {
        const int row_length = row.size();
        for (int i = 0; i < row_length; ++i) {
            auto c = row[i];
            auto node = std::make_shared<Node>(get_height(c), i, j);
            if (node->height == 0) lowest_nodes.push_back(node);
            if (c == 'S') start = node;
            else if (c == 'E') end = node;
            nodes.push_back(node);
            set_neighbours(nodes, node, row_length);
        }
        ++j;
    }
    return { start, end, nodes };
}

int f(std::shared_ptr<Node> node) {
    return node->distance + node->to_end;
}

int distance(const std::shared_ptr<Node>& from, const std::shared_ptr<Node>& to) {
    return std::abs(from->height - to->height);
}

int p1(const std::shared_ptr<Node>& start, const std::shared_ptr<Node>& end) {
    std::set<std::shared_ptr<Node>> todo{start};
    auto cmp = [](std::shared_ptr<Node> a, std::shared_ptr<Node> b) {
        return f(a) > f(b);
    };
    std::priority_queue queue{todo.begin(), todo.end(), cmp};
    std::unordered_set<std::shared_ptr<Node>> done;

    while (!todo.empty()) {
        auto element = queue.top();
        queue.pop();
        todo.erase(element);

        for (auto& neighbour : element->neighbours) {
            if (todo.contains(neighbour)) continue;
            if (done.contains(neighbour)) continue;
            if (neighbour->height - element->height > 1) continue;
            auto distance_to = element->distance + 1;
            auto to_end = distance(neighbour, end);
            neighbour->distance = distance_to;
            neighbour->to_end = to_end;
            if (neighbour == end) return distance_to;
            queue.push(neighbour);
            todo.insert(neighbour);
        }
        done.insert(element);
    }
    return 1e9;
}

std::vector<std::shared_ptr<Node>> lowest_nodes(const std::vector<std::shared_ptr<Node>>& nodes) {
    std::vector<std::shared_ptr<Node>> lowest;
    for (auto& node : nodes) {
        if (node->height == 0) lowest.push_back(node);
    }
    return lowest;
}

void reset_stats(const std::vector<std::shared_ptr<Node>>& nodes) {
    for (auto& node : nodes) {
        node->distance = 0;
    }
}

int p2(const std::vector<std::shared_ptr<Node>>& nodes, const std::shared_ptr<Node>& end) {
    auto lowest = lowest_nodes(nodes);
    int min = 1e9;
    for (auto& node : lowest) {
        reset_stats(nodes);
        auto dist = p1(node, end);
        min = std::min(min, dist);
    }
    return min;
}


int main() {
    std::ifstream f{"../12/input"};
    auto [start, end, nodes] = process_input(f);
    std::cout << p1(start, end) << std::endl;
    std::cout << p2(nodes, end) << std::endl;
}
