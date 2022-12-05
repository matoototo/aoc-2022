#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <tuple>
#include <utility>


using commands = std::vector<std::string>;

std::tuple<int, int, int> decode_move(std::string_view sv) {
    std::string s1, s2, s3;
    int amount, from, to;
    std::stringstream ss;
    ss << sv;
    ss >> s1 >> amount >> s2 >> from >> s3 >> to;

    from -= 1;
    to -= 1;

    return {amount, from, to};
}

struct State {
    std::vector<std::deque<char>> cols;

    void move(const std::string_view& sv) {
        auto [amount, from, to] = decode_move(sv);

        for (auto i = 0; i < amount; ++i) {
            cols[to].push_front(cols[from].front());
            cols[from].pop_front();
        }
    }

    void batch_move(const std::string_view& sv) {
        auto [amount, from, to] = decode_move(sv);

        std::deque<char> tmp;
        for (auto i = 0; i < amount; ++i) {
            tmp.push_front(cols[from].front());
            cols[from].pop_front();
        }

        for (auto& c : tmp) {
            cols[to].push_front(c);
        }
    }

    std::string top() const {
        std::stringstream ss;
        for (auto& col : cols)
            ss << col.front();

        return ss.str();
    }
};



State process_state(std::ifstream& f) {
    std::string curr_line;
    State s;

    bool first = true;

    while (std::getline(f, curr_line)) {
        if (first) {
            first = false;
            s.cols.resize((curr_line.length()+1)/4);
        }
        if (curr_line.length() <= 2) break;
        if (curr_line.find("[") != -1) {
            for (auto i = 1; i < curr_line.length(); i += 4) {
                if (auto c = curr_line.at(i); c != ' ') {
                    s.cols[(i-1) / 4].push_back(c);
                }
            }
        }
    }

    return s;
}

std::pair<State, commands> process_input(std::ifstream& f) {
    State s = process_state(f);
    commands c;

    std::string curr_line;
    while (std::getline(f, curr_line))
        c.push_back(curr_line);

    return {s, c};

}

std::string p1(State s, const commands& c) {
    for (auto& command : c) {
        s.move(command);
    }
    return s.top();
}

std::string p2(State s, const commands& c) {
    for (auto& command : c) {
        s.batch_move(command);
    }
    return s.top();
}

int main() {
    std::ifstream f{"input"};
    auto [state, commands] = process_input(f);
    std::cout << p1(state, commands) << std::endl;
    std::cout << p2(state, commands) << std::endl;
    return 0;
}
