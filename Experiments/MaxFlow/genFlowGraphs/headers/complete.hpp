#pragma once

#include <random>
#include <string>
#include <limits>
#include <utility>
#include <iostream>
#include <algorithm>

void genCompleteFlowGraph(int n, const long long &seed, const std::string &filename) {
    auto gen = std::mt19937(seed);
    auto cap = std::uniform_int_distribution <int>(1, std::numeric_limits <int>::max() - 1);

    using edge = std::pair <std::pair <int, int>, int>;

    std::vector <edge> edges;
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            edges.emplace_back(std::make_pair(i, j), cap(gen));
            edges.emplace_back(std::make_pair(j, i), cap(gen));
        }
    }

    for (int i = 1; i <= n; ++i) {
        edges.emplace_back(std::make_pair(0, i), cap(gen));     // source to node
        edges.emplace_back(std::make_pair(i, n + 1), cap(gen)); // node to sink
    }

    (void)!freopen(filename.c_str(), "w", stdout);

    std::cout << n + 2 << ' ' << edges.size() << ' ' << 0 << ' ' << n + 1 << '\n';
    for (const auto &e : edges) std::cout << e.first.first << ' ' << e.first.second << ' ' << e.second << '\n';
}