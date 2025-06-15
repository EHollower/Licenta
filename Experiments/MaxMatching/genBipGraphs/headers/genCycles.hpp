// https://github.com/yosupo06/library-checker-problems/blob/master/graph/bipartitematching/gen/cycle.cpp
#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

void genCycle(int L, int R, const long long &seed, const std::string &filename) {
    std::mt19937_64 rng(seed);

    int n = std::min(L, R);
    using p = std::pair <int, int>;

    std::vector <p> edges;
    for (int i = 0; i < n; ++i) {
        edges.emplace_back(i, i);
        edges.emplace_back(i, (i + 1) % n);
    }

    const int m = (int)edges.size();
    shuffle(edges.begin(), edges.end(), rng);

    std::vector <int> left(n), right(n);
    iota(left.begin(), left.end(), 0);
    iota(right.begin(), right.end(), 0);

    shuffle(left.begin(), left.end(), rng);
    shuffle(right.begin(), right.end(), rng);

    (void)!freopen(filename.c_str(), "w", stdout);

    std::cout << n << ' ' << n << ' ' << m << '\n';
    for (auto& e : edges) std::cout << left[e.first] << ' ' << right[e.second] << '\n';
}