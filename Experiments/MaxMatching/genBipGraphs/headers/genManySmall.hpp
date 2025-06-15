// https://github.com/yosupo06/library-checker-problems/blob/master/graph/bipartitematching/gen/many_smalls.cpp
#pragma once

#include <set>
#include <vector>
#include <random>
#include <utility>
#include <iostream>
#include <algorithm>

void genManySmall(int L, int R, int M, const long long &seed, const std::string &filename) {
    std::mt19937 rng(seed);

    int l = 0, r = 0, m = 0;
    using P = std::pair <int, int>;
    std::vector <P> edges;
    while (l < L - 100 && r < R - 100 && m < M - 100) {
        int nl = std::uniform_int_distribution<int>(1, 20)(rng);
        int nr = std::uniform_int_distribution<int>(1, 20)(rng);
        int nm = std::uniform_int_distribution<int>(1, std::min(nl * nr, 100))(rng);

        std::set <P> ns;
        while (int(ns.size()) < nm) {
            int u = std::uniform_int_distribution<int>(l, l + nl - 1)(rng);
            int v = std::uniform_int_distribution<int>(r, r + nr - 1)(rng);
            ns.insert({u, v});
        }

        for (auto p: ns) edges.push_back(p);
        l += nl; r += nr; m += nm;
    }

    m = int(edges.size());
    shuffle(edges.begin(), edges.end(), rng);

    (void)!freopen(filename.c_str(), "w", stdout);
    std::cout << l << ' ' << r << ' ' << m << '\n';
    for (const auto& [a, b] : edges) {
        std::cout << a << ' ' << b << '\n';
    }
}
