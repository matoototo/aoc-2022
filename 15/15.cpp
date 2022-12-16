#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <memory>
#include <optional>
#include <unordered_set>
#include <sstream>

using Position = std::pair<int, int>;
int manhattan(const Position& a, const Position& b);

Position add(const Position& a, const Position& b) {
    return {a.first + b.first, a.second + b.second};
}

class PairHash {
public:
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U>& p) const
    {
        std::size_t h1 = std::hash<T>()(p.first);
        std::size_t h2 = std::hash<U>()(p.second);
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};

struct Beacon {
    Position position;
};

struct Sensor {
    Sensor(Position pos, Position beacon_pos): position(pos), beacon_position(beacon_pos) {}
    Position position;
    Position beacon_position;
    int range() const {
        if (!range_) range_ = manhattan(position, beacon_position);
        return *range_;
    }
    std::unordered_set<Position, PairHash>& rim_points() const {
        int size = 4000000;
        if (!rim_points_.size()) {
            int sum = range() + 1;
            for (auto i = 0; i <= sum; ++i) {
                auto target = add(position, Position{i, sum-i});
                if (target.first >= 0 && target.first <= size && target.second >= 0 && target.second <= size)
                    rim_points_.insert(target);
                target = add(position, Position{-i, sum-i});
                if (target.first >= 0 && target.first <= size && target.second >= 0 && target.second <= size)
                    rim_points_.insert(target);
                target = add(position, Position{-i, -sum-i});
                if (target.first >= 0 && target.first <= size && target.second >= 0 && target.second <= size)
                    rim_points_.insert(target);
                target = add(position, Position{-i, -sum+i});
                if (target.first >= 0 && target.first <= size && target.second >= 0 && target.second <= size)
                    rim_points_.insert(target);
            }
        }
        return rim_points_;
    }
    private:
    mutable std::optional<int> range_;
    mutable std::unordered_set<Position, PairHash> rim_points_;
};


std::pair<std::vector<Sensor>, std::vector<Beacon>> process_input(std::ifstream& f) {
    std::string line, _;
    std::vector<std::pair<int, int>> pairs;
    std::vector<Sensor> sensors;
    std::vector<Beacon> beacons;
    while (std::getline(f, line)) {

        auto x_start = line.find_first_of("x") + 2;
        auto x_end = line.find_first_of(",");

        auto y_start = line.find_first_of("y") + 2;
        auto y_end = line.find_first_of(":");

        auto x_beacon_start = line.find_last_of("x") + 2;
        auto x_beacon_end = line.find_last_of(",");
        auto y_beacon_start = line.find_last_of("y") + 2;
        auto y_beacon_end = line.length();


        int x = std::stoi(line.substr(x_start, x_end - x_start));
        int y = std::stoi(line.substr(y_start, y_end - y_start));
        int beacon_x = std::stoi(line.substr(x_beacon_start, x_beacon_end - x_beacon_start));
        int beacon_y = std::stoi(line.substr(y_beacon_start, y_beacon_end - y_beacon_start));

        sensors.push_back(Sensor{{x, y}, {beacon_x, beacon_y}});
        beacons.push_back(Beacon{{beacon_x, beacon_y}});
    }
    return {sensors, beacons};
}

std::unordered_set<Position, PairHash> get_positions(const std::vector<Sensor>& sensors) {
    std::unordered_set<Position, PairHash> positions;

    for (auto& sensor : sensors) {
        positions.insert(sensor.position);
        positions.insert(sensor.beacon_position);
    }

    return positions;
}

std::pair<int, int> min_max_x(const std::vector<Sensor>& sensors) {
    std::optional<int> min, max;
    for (auto& sensor: sensors) {
        auto [x, y] = sensor.position;
        if (!min) min = x;
        if (!max) max = x;
        min = std::min(*min, x - sensor.range());
        max = std::max(*max, x + sensor.range());
    }
    return {*min, *max};
}

std::unordered_set<Position, PairHash> get_beacon_positions(const std::vector<Sensor>& sensors) {
    std::unordered_set<Position, PairHash> beacon_positions;

    for (auto& sensor : sensors) {
        beacon_positions.insert(sensor.beacon_position);
    }

    return beacon_positions;
}

int manhattan(const Position& a, const Position& b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

bool in_range(const Sensor& sensor, const Position& probe) {
    return manhattan(sensor.position, probe) <= sensor.range();
}

int cannot_contain(const std::vector<Sensor>& sensors, const Position& probe) {
    // cannot contain if manhattan distance <= that of sensor
    for (auto& sensor : sensors) {
        if (in_range(sensor, probe)) {
            return 1;
        }
    }
    return 0;
}

int p1(const std::vector<Sensor>& sensors) {
    auto all_positions = get_positions(sensors);
    auto [min_x, max_x] = min_max_x(sensors);

    int count = 0;
    for (auto x = min_x; x <= max_x; ++x) {
        Position probe{x, 2000000};
        if (all_positions.contains(probe)) continue;
        count += cannot_contain(sensors, probe);
    }

    return count;
}

long long p2(const std::vector<Sensor>& sensors) {
    int size = 4000000;
    int i = 0;
    std::unordered_set<Position, PairHash> rim_points;
    for (auto& sensor : sensors) {
        std::cout << ++i << std::endl;
        rim_points.insert(sensor.rim_points().begin(), sensor.rim_points().end());
    }

    for (auto& sensor : sensors) {
        std::unordered_set<Position, PairHash> no_overlap;
        for (auto& point : rim_points) {
            auto& [x, y] = point;
            if (x > 0 && y > 0 && x < size && y < size)
                if (!in_range(sensor, point)) no_overlap.insert(point);
        }
        rim_points = no_overlap;
    }
    for (auto& [x, y] : rim_points) {
        if (x > 0 && y > 0 && x < size && y < size) {
            return (long long)x*4000000 + y;
        }
    }

    return 0;
}

int main() {
    std::ifstream f{"../15/input"};
    auto [sensors, beacons] = process_input(f);
    std::cout << p1(sensors) << std::endl;
    std::cout << p2(sensors) << std::endl;
}
