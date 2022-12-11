#include <fstream>
#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <sstream>
#include <memory>

struct State {
    int reg = 1;
    int cycle = 1;
    std::vector<int> history;
    void add(int x, int latency) {
        for (int i = 0; i < latency; ++i) {
            history.push_back(reg);
        }
        reg += x;
        cycle += latency;
    }
};

struct Instruction {
    virtual void operator()(State& s) = 0;
};

struct NOOP : Instruction {
    virtual void operator()(State& s) {
        s.add(0, 1);
    }
};

struct ADDX : Instruction {
    ADDX(int x): x(x) {}
    int x;
    virtual void operator()(State& s) {
        s.add(x, 2);
    }
};

std::vector<std::shared_ptr<Instruction>> process_input(std::ifstream& f) {
    std::string line, operation;
    std::vector<std::shared_ptr<Instruction>> instructions;
    while (std::getline(f, line)) {
        std::stringstream ss;
        ss << line;
        ss >> operation;
        if (operation == "noop") {
            instructions.emplace_back(std::make_shared<NOOP>());
        } else {
            int argument;
            ss >> argument;
            instructions.emplace_back(std::make_shared<ADDX>(argument));
        }
    }
    return instructions;
}

int p1(const std::vector<std::shared_ptr<Instruction>>& instructions, State& s) {
    for (auto inst : instructions) {
        (*inst)(s);
    }

    int strength = 0;

    for (int i = 0; i < 6; ++i) {
        auto cycle = 20 + i * 40;
        auto index = cycle - 1;
        if (s.history.size() > index) {
            strength += cycle * s.history.at(index);
        }
    }

    return strength;
}

std::string p2(const std::vector<std::shared_ptr<Instruction>>& instructions, State& s) {
    std::stringstream ss;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 40; ++j) {
            auto index = i * 40 + j;
            auto reg_value = s.history[index];
            if (std::abs(reg_value - j) <= 1) ss << '#';
            else ss << ' ';
        }
        ss << '\n';
    }

    return ss.str();
}

int main() {
    std::ifstream f{"../10/input"};
    State s;
    auto instructions = process_input(f);
    std::cout << p1(instructions, s) << std::endl;
    std::cout << p2(instructions, s) << std::endl;
}
