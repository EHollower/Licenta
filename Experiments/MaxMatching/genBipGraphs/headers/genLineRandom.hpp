// https://github.com/yosupo06/library-checker-problems/blob/master/graph/bipartitematching/gen/line_random.cpp
#pragma once

#include <vector>
#include <random>
#include <utility>
#include <iostream>
#include <algorithm>

void genLineRnd(int L, int R, const long long &seed, const std::string &filename) {
    std::mt19937 rng(seed);

    int n = std::min(L, R);

    using P = std::pair <int, int>;
    std::vector <P> edges;

    for (int i = 0; i < n; i++) {
    edges.push_back({i, i});
        if (i) edges.push_back({i, i - 1});
    }
    int m = int(edges.size());
    shuffle(edges.begin(), edges.end(), rng);

    std::vector <int> left(n), right(n);
    iota(left.begin(), left.end(), 0);
    iota(right.begin(), right.end(), 0);

    shuffle(left.begin(), left.end(), rng);
    shuffle(right.begin(), right.end(), rng);

    (void)!freopen(filename.c_str(), "w", stdout);
    std::cout << n << ' ' << n << ' ' << m << '\n';
    for (const auto& [a, b] : edges) {
        std::cout << left[a] << ' ' << right[b] << '\n';
    }
}