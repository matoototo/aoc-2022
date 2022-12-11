#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
#include <functional>
#include <string>
#include <sstream>
#include <memory>

struct Monkey {
    std::vector<long long> items;
    std::function<long long(long long, long long)> operation;
    bool second_old{false};
    long long second;
    long long divisor;
    size_t true_monkey, false_monkey;
    long long inspected{0};
};

std::vector<Monkey> process_input(std::ifstream& f) {
    std::vector<Monkey> monkeys;
    std::string s;
    while (std::getline(f, s)) {
        std::stringstream ss;
        ss << s;
        if (s.starts_with("Monkey")) {
            monkeys.emplace_back();
        }
        else if (s.starts_with("  Starting")) {
            std::string s1, s2;
            int x;
            char comma;
            ss >> s1 >> s2;
            while (ss) {
                ss >> x >> comma;
                monkeys.back().items.push_back(x);
            }
        } else if (s.starts_with("  Operation")) {
            std::string s1, s2, s3, first, operation, second;
            ss >> s1 >> s2 >> s3 >> first >> operation >> second;
            if (second == "old") monkeys.back().second_old = true;
            else monkeys.back().second = std::stoi(second);
            if (operation == "+") {
                monkeys.back().operation = [](long long a, long long b) { return a + b; };
            } else if (operation == "*") {
                monkeys.back().operation = [](long long a, long long b) { return a * b; };
            }
        } else if (s.starts_with("  Test")) {
            std::string s1, s2, s3;
            int n;
            ss >> s1 >> s2 >> s3 >> n;
            monkeys.back().divisor = n;
        } else if (s.starts_with("    If true")) {
            std::string _;
            int monke;
            ss >> _ >> _ >> _ >> _ >> _ >> monke;
            monkeys.back().true_monkey = monke;
        } else if (s.starts_with("    If false")) {
            std::string _;
            int monke;
            ss >> _ >> _ >> _ >> _ >> _ >> monke;
            monkeys.back().false_monkey = monke;
        }
    }
    return monkeys;
}

long long manage_p1(long long item) {
    return item / 3;
}

long long manage_p2(long long item, const std::vector<Monkey>& monkeys) {
    long long cum = 1;
    for (auto& monkey : monkeys) cum *= monkey.divisor;
    return item % cum;
}

void round(std::vector<Monkey>& monkeys, bool p1 = true) {
    for (auto& monkey : monkeys) {
        for (auto& item : monkey.items) {
            ++monkey.inspected;
            auto second = monkey.second_old ? item : monkey.second;
            item = monkey.operation(item, second);
            if (p1) item = manage_p1(item);
            else item = manage_p2(item, monkeys);
            if (item % monkey.divisor == 0) {
                monkeys.at(monkey.true_monkey).items.push_back(item);
            } else {
                monkeys.at(monkey.false_monkey).items.push_back(item);
            }
        }
        monkey.items = {};
    }
}

long long p1(std::vector<Monkey>& monkeys) {
    for (int i = 0; i < 20; ++i) {
        round(monkeys, true);
    }
    std::sort(monkeys.begin(), monkeys.end(), [](auto a, auto b) { return a.inspected > b.inspected; });
    return monkeys[0].inspected * monkeys[1].inspected;
}

long long p2(std::vector<Monkey>& monkeys) {
    for (int i = 0; i < 10000; ++i) {
        round(monkeys, false);
    }
    std::sort(monkeys.begin(), monkeys.end(), [](auto a, auto b) { return a.inspected > b.inspected; });
    return monkeys[0].inspected * monkeys[1].inspected;
}

int main() {
    std::ifstream f{"../11/input"};
    auto monkeys = process_input(f);
    std::cout << p1(monkeys) << std::endl;
    std::ifstream f2{"../11/input"};
    monkeys = process_input(f2);
    std::cout << p2(monkeys) << std::endl;
}
