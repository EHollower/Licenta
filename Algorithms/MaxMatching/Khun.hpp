#pragma once

#include "../../Utility/clock.hpp"

#include <vector>
#include <string>

struct Khun {
    int L;
    std::vector <std::basic_string <int>> G;
    std::vector <int> mt;
    std::vector <bool> used;

    Khun(int L, int R) : L(L), G(L), mt(R), used(L) {}

    inline void addEdge(int u, int v) { G[u].push_back(v); }

    bool try_kuhn(int u) {
        if (used[u]) return false;

        used[u] = true;
        for (const auto &v : G[u]) {
            if (mt[v] == -1 || try_kuhn(mt[v])) {
                mt[v] = u; 
                return true;
            }
        }
        return false;
    }

    int MaxMatching() {
        TLE::start();
        fill(mt.begin(), mt.end(), -1);

        int res = 0;
        for (int v = 0; v < L; ++v) {
            fill(used.begin(), used.end(), false);
            res += try_kuhn(v);
            if (TLE::TLE()) return -1;
        }

        return res;
    }
};

/*
# Complxity: O(VE) ~ O(n_1^2n_2) (good when the first part is small)

# Utility:
0th index for both parts
KhunArb(L, R) -> number of vertices in the first and second parts, initializes algorithm
addEdge(u, v) -> adds an edge from u in the first part to v in the second part
MaxMatching() -> returns the size of the maximum matching

# References:
https://infoarena.ro/problema/cuplaj
https://en.wikipedia.org/wiki/Hungarian_algorithm
https://link.springer.com/chapter/10.1007/978-3-540-68279-0_2
https://cp-algorithms.com/graph/kuhn_maximum_bipartite_matching.html
*/