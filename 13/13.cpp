#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <memory>
#include <optional>
#include <set>
#include <sstream>

struct Node;
std::string to_string(const Node& node);

struct Node {
    Node() {}
    Node(int v): value(v) {}
    std::optional<int> value{};
    std::vector<Node> children;
    bool operator==(const Node &other) const {
        return to_string(*this) == to_string(other);
    }
};

std::string to_string(const Node& node) {
    std::stringstream ss;
    if (node.value) {
        ss << *node.value << ',';
    } else {
        ss << '[';
        for (auto& n : node.children) {
            ss << to_string(n);
        }
        ss << ']';
    }
    return ss.str();
}

bool char_is_number(char c) {
    return (c - '0' >= 0 && c - '0' <= 9);
}

std::string_view next_list(std::string_view sv, size_t start) {
    int counter = 0;
    for (auto it = sv.begin() + start; it != sv.end(); ++it) {
        if (*it == '[') ++counter;
        else if (*it == ']') --counter;

        if (counter == 0) {
            return sv.substr(start, it - sv.begin() - start + 1);
        }
    }
    return "";
}

Node process_line(std::string_view sv) {
    sv = sv.substr(1, sv.size() - 2);

    Node root{};
    std::optional<int> current;
    for (auto it = sv.begin(); it != sv.end(); ++it) {
        if (*it == '[') {
            auto next = next_list(sv, it - sv.begin());
            root.children.push_back(process_line(next));
            it += next.size();
            if (it == sv.end()) break;
        } else if (char_is_number(*it)) {
            if (!current) current = 0;
            current = current.value() * 10;
            current = current.value() + *it - '0';
        } else {
            root.children.emplace_back(current.value());
            current.reset();
        }
    }
    if (current) root.children.emplace_back(current.value());
    return root;
}

std::vector<std::pair<Node, Node>> process_input(std::ifstream& f) {
    std::string s1, s2;
    std::vector<std::pair<Node, Node>> pairs;
    while (std::getline(f, s1) && std::getline(f, s2)) {
        pairs.push_back(std::make_pair(process_line(s1), process_line(s2)));
        f.ignore();
    }
    return pairs;
}

Node to_list(const Node& node) {
    Node root{};
    root.children.push_back(node);
    return root;
}

std::pair<bool, bool> compare(const Node& first, const Node& second) {
    const auto should_continue{std::make_pair(true, false)};
    const auto in_right_order{std::make_pair(false, true)};
    const auto in_wrong_order{std::make_pair(false, false)};

    if (first.value && second.value) {
        if (first.value == second.value) return should_continue;
        return { false, first.value < second.value };
    } else if (first.value) {
        return compare(to_list(first), second);
    } else if (second.value) {
        return compare(first, to_list(second));
    } else {
        auto it1 = first.children.begin();
        auto it2 = second.children.begin();
        for (; it1 != first.children.end() && it2 != second.children.end(); ++it1, ++it2) {
            auto [go_on, in_order] = compare(*it1, *it2);
            if (!go_on) return { false, in_order };
        }
        if (it1 == first.children.end() && it2 == second.children.end()) return should_continue;
        else if (it1 == first.children.end()) return in_right_order;
        else return in_wrong_order;
    }
}

int p1(const std::vector<std::pair<Node,Node>>& pairs) {
    auto i = 1;
    auto index_sum = 0;
    for (auto& [first, second] : pairs) {
        auto [go_on, in_order] = compare(first, second);
        if (in_order) index_sum += i;
        ++i;
    }
    return index_sum;
}

std::vector<Node> unpack(std::vector<std::pair<Node, Node>>& pairs) {
    std::vector<Node> unpacked;
    for (auto& [first, second] : pairs) {
        unpacked.push_back(first);
        unpacked.push_back(second);
    }
    return unpacked;
}

int p2(std::vector<std::pair<Node, Node>>& pairs) {
    auto nodes = unpack(pairs);

    auto first_divider = process_line("[[2]]");
    nodes.push_back(first_divider);
    auto second_divider = process_line("[[6]]");
    nodes.push_back(second_divider);

    auto cmp = [](auto& first, auto& second) { return compare(first, second).second; };

    std::sort(nodes.begin(), nodes.end(), cmp);

    int decoder_key = 1;
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        if (*it == first_divider) decoder_key *= (it - nodes.begin() + 1);
        else if (*it == second_divider) decoder_key *= (it - nodes.begin() + 1);
    }

    return decoder_key;
}

int main() {
    std::ifstream f{"../13/input"};
    auto v = process_input(f);
    std::cout << p1(v) << std::endl;
    std::cout << p2(v) << std::endl;
}
