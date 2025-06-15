// https://github.com/yosupo06/library-checker-problems/blob/master/graph/bipartitematching/gen/issue1068_large.cpp
#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

void genLarge(int L_MAX, int R_MAX, int M_MAX, const long long &seed, const std::string &filename) {
    std::mt19937_64 rng(seed);

    int n = std::min(L_MAX, R_MAX) / 6;
    n = std::min(n, (M_MAX + 7) / 16);

    using p = std::pair <int, int>;

    std::vector <p> edge;
    for (int i = 0; i < 2 * n - 1; ++i) {
        edge.emplace_back(2 * i + 2, 2 * i + 0);
        edge.emplace_back(2 * i + 2, 2 * i + 1);
        edge.emplace_back(2 * i + 3, 2 * i + 0);
        edge.emplace_back(2 * i + 3, 2 * i + 1);
    }
    for (int i = 2 * n; i < 3 * n; ++i) {
        edge.emplace_back(2 * i + 0, 2 * i - 1);
        edge.emplace_back(2 * i + 0, 2 * i - 2);
        if (i < 3 * n - 1) edge.emplace_back(2 * i + 1, 2 * i + 0);
        edge.emplace_back(2 * i + 1, 2 * i - 1);
    }

    int m = (int)edge.size();
    int L = 6 * n, R = 6 * n - 2;

    if (seed % 2 == 1) {
        // swap LR
        std::swap(L, R);
        for (auto& [a, b]: edge) { std::swap(a, b); }
    }

    if (seed % 3 == 1) { reverse(edge.begin(), edge.end()); }
    if (seed % 3 == 2) { shuffle(edge.begin(), edge.end(), rng); }

    (void)!freopen(filename.c_str(), "w", stdout);

    std::cout << L << " " << R << " " << m << "\n";
    for (const auto& [a, b]: edge) std::cout << a << " " << b << "\n";
}