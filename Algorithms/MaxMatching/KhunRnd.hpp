#pragma once

#include "../../Utility/clock.hpp"

#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>

struct KhunRnd {
    int L;
    std::vector <std::basic_string <int>> G;
    std::vector <int> l, r;
    std::vector <bool> used;
    std::mt19937 rnd;

    KhunRnd(int L, int R) : L(L), G(L), l(L), r(R), used(L) {
        rnd.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    }

    inline void addEdge(int u, int v) { G[u].push_back(v); }

    bool try_kuhn(int u) {
        if (used[u]) return false;

        used[u] = true;
        for (const auto &v : G[u]) {
            if (r[v] == -1) {
                l[u] = v;
                r[v] = u;
                return true;
            }
        }

        for (const auto &v : G[u]) {
            if (try_kuhn(r[v])) {
                l[u] = v;
                r[v] = u;
                return true;
            }
        }

        return false;
    }

    int MaxMatching() {
        TLE::start();
        for (int i = 0; i < L; ++i) shuffle(G[i].begin(), G[i].end(), rnd);

        std::vector <int> order(L);
        iota(order.begin(), order.end(), 0);
        shuffle(order.begin(), order.end(), rnd);

        fill(l.begin(), l.end(), -1);
        fill(r.begin(), r.end(), -1);

        for (int change = 1; change; ) {
            change = 0;
            std::fill(used.begin(), used.end(), false);
            for (const auto &i : order) {
                if (l[i] == -1) change |= try_kuhn(i);
            }

            if (TLE::TLE()) return -1;
        }   

        int res = 0;
        for (int i = 0; i < L; ++i) res += (l[i] != -1);
        return res;
    }
};


/*
# Complxity: O(VE) ~ O(n_1^2n_2) (good when the first part is small) 
- Heuristic (for random graphs) + repeat till no changes + shuffle

# Utility:
0th index for both parts
KhunArb(L, R) -> number of vertices in the first and second parts, initializes algorithm
addEdge(u, v) -> adds an edge from u in the first part to v in the second part
MaxMatching() -> returns the size of the maximum matching

# References:
https://infoarena.ro/problema/cuplaj
https://codeforces.com/blog/entry/58048
https://en.wikipedia.org/wiki/Hungarian_algorithm
https://link.springer.com/chapter/10.1007/978-3-540-68279-0_2
https://cp-algorithms.com/graph/kuhn_maximum_bipartite_matching.html
*/