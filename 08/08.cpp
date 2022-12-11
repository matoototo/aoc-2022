#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

struct Node {
    Node(int h): height(h) {}
    int height;
    std::vector<std::shared_ptr<Node>> neighbours{nullptr, nullptr, nullptr, nullptr}; // up-right-down-left
    std::vector<bool> visible{false, false, false, false};
    enum {
        UP = 0,
        RIGHT = 1,
        DOWN = 2,
        LEFT = 3
    };
};


void set_neighbours(std::vector<std::shared_ptr<Node>>& nodes, std::shared_ptr<Node> node, const int row_length) {
    if (nodes.size() > row_length) {
        auto& up = nodes[nodes.size() - row_length - 1];
        up->neighbours[Node::DOWN] = node;
        node->neighbours[Node::UP] = up;
    }
    if (nodes.size() % row_length != 1) {
        auto& left = nodes[nodes.size() - 2];
        left->neighbours[Node::RIGHT] = node;
        node->neighbours[Node::LEFT] = left;
    }
}

std::vector<std::shared_ptr<Node>> process_input(std::ifstream& f) {
    std::string row;
    std::vector<std::shared_ptr<Node>> nodes;
    while (std::getline(f, row)) {
        const int row_length = row.size();
        for (auto c : row) {
            auto node = std::make_shared<Node>(c - '0');
            nodes.push_back(node);
            set_neighbours(nodes, node, row_length);
        }
    }
    return nodes;
}

int opposite_direction(int direction) {
    if (direction == Node::UP) return Node::DOWN;
    else if (direction == Node::RIGHT) return Node::LEFT;
    else if (direction == Node::DOWN) return Node::UP;
    else if (direction == Node::LEFT) return Node::RIGHT;
    throw std::runtime_error("Invalid direction given.");
}

void probe(std::shared_ptr<Node> node, int direction) {
    auto opposite = opposite_direction(direction);
    auto max_height = node->height;
    while (auto next = node->neighbours[direction]) {
        max_height = std::max({node->height, max_height});
        if (max_height < next->height) {
            next->visible[opposite] = true;
        }
        node = next;
    }
}

size_t p1(const std::vector<std::shared_ptr<Node>>& nodes) {
    // go from top_left around the border and send probes towards middle
    auto current_node = nodes[0];
    current_node->visible[Node::UP] = true;
    current_node->visible[Node::LEFT] = true;
    while (auto next = current_node->neighbours[Node::RIGHT]) {
        current_node = next;
        current_node->visible[Node::UP] = true;
        probe(current_node, Node::DOWN);
    }

    current_node->visible[Node::UP] = true;
    current_node->visible[Node::RIGHT] = true;
    while (auto next = current_node->neighbours[Node::DOWN]) {
        current_node = next;
        current_node->visible[Node::RIGHT] = true;
        probe(current_node, Node::LEFT);
    }

    current_node->visible[Node::DOWN] = true;
    current_node->visible[Node::RIGHT] = true;
    while (auto next = current_node->neighbours[Node::LEFT]) {
        current_node = next;
        current_node->visible[Node::DOWN] = true;
        probe(current_node, Node::UP);
    }

    current_node->visible[Node::DOWN] = true;
    current_node->visible[Node::LEFT] = true;
    while (auto next = current_node->neighbours[Node::UP]) {
        current_node = next;
        current_node->visible[Node::LEFT] = true;
        probe(current_node, Node::RIGHT);
    }

    size_t count{0};
    for (auto& node : nodes)
        if (std::any_of(node->visible.begin(), node->visible.end(), [](bool e){ return e; })) {
            count += 1;
        }
    return count;
}

size_t seen_trees(std::shared_ptr<Node> node, int direction) {
    auto height = node->height;
    size_t count = 0;
    while (auto next = node->neighbours[direction]) {
        ++count;
        if (next->height >= height) break;
        node = next;
    }
    return count;
}

long long scenic_score(std::shared_ptr<Node> node) {
    long long score = 1;
    for (auto i = 0; i < 4; ++i) {
        score *= seen_trees(node, i);
    }
    return score;
}

long long p2(const std::vector<std::shared_ptr<Node>>& nodes) {
    long long max = 0;
    for (auto& node : nodes) {
        max = std::max({scenic_score(node), max});
    }
    return max;
}

int main() {
    std::ifstream f{"../08/input"};
    auto nodes = process_input(f);
    std::cout << p1(nodes) << std::endl;
    std::cout << p2(nodes) << std::endl;
}
