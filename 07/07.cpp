#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <deque>
#include <algorithm>

struct Node {
    Node(long long s, const std::string& n): size(s), name(n), children{} {}
    long long size;
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<Node>> children;
    std::shared_ptr<Node> parent;

    long long tot_size = 0;
    long long dir_size() {
        if (size != 0 || tot_size != 0) return tot_size;
        for (auto& [k, v] : children) {
            if (v->size == 0) tot_size += v->dir_size();
            else tot_size += v->size;
        }
        return tot_size;
    }
};

std::pair<std::string, std::string> parse_command(const std::string_view& sv) {
    std::stringstream ss;
    std::string s1, s2;
    ss << sv;
    ss.ignore(1);
    ss >> s1 >> s2;
    return {s1, s2};
}

std::shared_ptr<Node> parse_listing(const std::string_view& sv) {
    std::stringstream ss;
    std::string name;
    int size;
    ss << sv;
    if (sv.size() && sv.starts_with("dir")) {
        size = 0;
        ss >> name >> name;
    } else {
        ss >> size >> name;
    }
    return std::make_shared<Node>(size, name);
}

std::shared_ptr<Node> process_input(std::ifstream& f) {
    std::string line;
    auto current = std::make_shared<Node>(0, "/");
    auto root = current;
    while (std::getline(f, line)) {
        if (line.size() && line[0] == '$') {
            auto [command, target] = parse_command(line);
            if (command == "cd") {
                if (target == "/") current = root;
                else if (target == "..") current = current->parent;
                else current = current->children[target];
            }
        } else {
            auto child{parse_listing(line)};
            current->children[child->name] = child;
            child->parent = current;
        }
    }
    return root;
}

long long p1(std::shared_ptr<Node> root) {
    std::vector<long long> sizes;
    std::vector<std::shared_ptr<Node>> todo{root};

    while (!todo.empty()) {
        auto back = todo.back();
        todo.pop_back();
        for (auto& [k, v] : back->children) {
            todo.push_back(v);
        }
        sizes.push_back(back->dir_size());
    }

    std::sort(sizes.begin(), sizes.end());
    long long sum = 0;
    for (auto s : sizes) {
        if (s > 100000) break;
        sum += s;
    }
    return sum;
}

long long p2(std::shared_ptr<Node> root) {
    using ntype = std::pair<std::shared_ptr<Node>, long long>;
    std::vector<std::pair<std::shared_ptr<Node>, long long>> sizes;
    std::vector<std::shared_ptr<Node>> todo{root};

    while (!todo.empty()) {
        auto back = todo.back();
        todo.pop_back();
        for (auto& [k, v] : back->children) {
            todo.push_back(v);
        }
        sizes.push_back(std::make_pair(back, back->dir_size()));
    }

    std::sort(sizes.begin(), sizes.end(), [](ntype f, ntype s) { return f.second < s.second; });

    auto needed = 30000000 - (70000000 - root->tot_size);
    for (auto& [ptr, s] : sizes) {
        if (s > needed) return s;
    }
    return -1;
}

int main() {
    std::ifstream f{"../07/input"};
    auto root = process_input(f);
    std::cout << p1(root) << std::endl;
    std::cout << p2(root) << std::endl;
}
