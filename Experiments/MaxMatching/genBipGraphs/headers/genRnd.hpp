// https://github.com/yosupo06/library-checker-problems/blob/master/graph/bipartitematching/gen/random.cpp
#pragma once

#include <set>
#include <vector>
#include <random>
#include <utility>
#include <iostream>
#include <algorithm>

void genRnd(int L, int R, int M, const long long &seed, const std::string &filename) {
    std::mt19937 rng(seed);

    int l = std::uniform_int_distribution<int>(1, L)(rng);
    int r = std::uniform_int_distribution<int>(1, R)(rng);
    int m = std::uniform_int_distribution<int>(1, M)(rng);

    using P = std::pair<int, int>;
    std::set <P> edges;
    for (int ph = 0; ph < m; ph++) {
        int u = std::uniform_int_distribution<int>(0, l - 1)(rng);
        int v = std::uniform_int_distribution<int>(0, r - 1)(rng);
        edges.insert({u, v});
    }
    m = int(edges.size());

    (void)!freopen(filename.c_str(), "w", stdout);
    std::cout << l << ' ' << r << ' ' << m << '\n';
    for (const auto& [a, b] : edges) {
        std::cout << a << ' ' << b << '\n';
    }
}