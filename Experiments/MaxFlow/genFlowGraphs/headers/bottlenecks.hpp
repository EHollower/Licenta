#pragma once

#include <random>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>

void genTestCaseBottle(int n, const long long &seed, const std::string &filename) {
    auto gen = std::mt19937(seed);
    auto cap = std::uniform_int_distribution<int>(1e9, 2e9);

    using edge = std::pair <std::pair <int, int>, int>;

    std::vector <edge> edges;
    for (int i = 0; i < n - 1; ++i) {
        edges.emplace_back(std::make_pair(i, i + 1), cap(gen));
        edges.emplace_back(std::make_pair(i, n - 1), cap(gen));
        for (int j = i + 3; j < n - 1; ++j) {
            edges.emplace_back(std::make_pair(i, j), 1);
            edges.emplace_back(std::make_pair(j, i), 1);
        }
    }

    (void)!freopen(filename.c_str(), "w", stdout);

    std::cout << n << ' ' << edges.size() << ' ' << 0 << ' ' << n - 1 << '\n';
    for (const auto &e : edges) std::cout << e.first.first << ' ' << e.first.second << ' ' << e.second << '\n';
}