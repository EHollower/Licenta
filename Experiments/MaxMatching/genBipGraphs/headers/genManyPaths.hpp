// https://github.com/yosupo06/library-checker-problems/blob/master/graph/bipartitematching/gen/line_random.cpp
#pragma once

#include <set>
#include <vector>
#include <random>
#include <utility>
#include <iostream>
#include <algorithm>

void genManyPaths(int L, int R, int M, const long long &seed, const std::string &filename) {
    std::mt19937 rng(seed);
    auto uniform = std::uniform_int_distribution<int>(20, 200);

    int n = std::min({L, R, M / 2});

    std::vector <int> partition;
    const int cnt = uniform(rng);
    for (int i = 0; i < cnt; ++i) {
        partition.push_back(std::uniform_int_distribution<int>(1, n - 1)(rng));
    }

    partition.push_back(0);
    partition.push_back(n);
    sort(partition.begin(), partition.end());
    partition.erase(unique(partition.begin(), partition.end()), partition.end());

    using P = std::pair <int, int>;
    std::set <P> edges;

    std::vector <int> left(n), right(n);
    iota(left.begin(), left.end(), 0);
    iota(right.begin(), right.end(), 0);

    shuffle(left.begin(), left.end(), rng);
    shuffle(right.begin(), right.end(), rng);

    for (int i = 1; i < (int)partition.size(); ++i) {
        const int len = partition[i] - partition[i - 1];
        shuffle(left.begin(), left.end(), rng);
        shuffle(right.begin(), right.end(), rng);
        for (int i = 0; i < len; ++i) {
            edges.insert({left[i], right[i]});
            edges.insert({left[i], right[i + 1]});
        }
    }

    std::vector <P> e(edges.begin(), edges.end());
    std::shuffle(e.begin(), e.end(), rng);

    (void)!freopen(filename.c_str(), "w", stdout);
    std::cout << n << ' ' << n << ' ' << (int)e.size() << '\n';
    for (const auto& [a, b] : e) {
        std::cout << a << ' ' << b << '\n';
    }
}
