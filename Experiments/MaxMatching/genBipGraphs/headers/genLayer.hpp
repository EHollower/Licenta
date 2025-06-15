// https://github.com/yosupo06/library-checker-problems/blob/master/graph/bipartitematching/gen/issue1068_large.cpp
#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

void genLayer(int B, const long long &seed, const std::string &filename) {
    std::mt19937_64 rng(seed);

    std::vector <std::pair<int, int> > edges;
    for (int i = 0; i <= B; i++) {
        for (int j = 0; j < (1 << B); j++) {
            edges.push_back({(i << B) + j, (i << B) + j});
        }
    }

    for (int i = 0; i < B; i++) {
        for (int j = 0; j < (1 << B); j++) {
            edges.push_back({((i + 1) << B) + j, (i << B) + j});
            edges.push_back({((i + 1) << B) + (j ^ (1 << i)), (i << B) + j});
        }
    }

    for (int i = 0; i < (1 << B); i++) {
        edges.push_back({((B + 1) << B) + i, (B << B) + i});
    }

    for (int i = 0; i < (1 << B); i++) {
        edges.push_back({i, ((B + 1) << B) + i});
    }

    int l = (B + 2) << B;
    int r = (B + 2) << B;
    int m = edges.size();

    if (seed % 3 == 0) {
        std::vector <int> new_idx_L(l), new_idx_R(r);
        for (int i = 0; i < l; ++i) new_idx_L[i] = i;
        for (int i = 0; i < r; ++i) new_idx_R[i] = i;
        shuffle(edges.begin(), edges.end(), rng);
        shuffle(new_idx_L.begin(), new_idx_L.end(), rng);
        shuffle(new_idx_R.begin(), new_idx_R.end(), rng);
        for (auto& [a, b]: edges) a = new_idx_L[a], b = new_idx_R[b];
    }
    
    if (seed % 3 == 2) { reverse(edges.begin(), edges.end()); }

    (void)!freopen(filename.c_str(), "w", stdout);

    std::cout << l << " " << r << " " << m << "\n";
    for (auto edge: edges) std::cout << edge.first << " " << edge.second << "\n";
}