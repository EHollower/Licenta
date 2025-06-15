#pragma once

#include "../../Utility/clock.hpp"

#include <queue>
#include <vector>
#include <limits>
#include <string>

struct HopcroftKarp {
    int L;
    std::vector <std::basic_string <int>> G;
    std::vector <int> l, r, d, done;

    HopcroftKarp(int L, int R) : L(L), G(L), l(L), r(R), d(L), done(L) {}

    inline void addEdge(int u, int v) { G[u].push_back(v); }

    bool bfs() {
        std::queue <int> q;
        for (int u = 0; u < L; ++u) {
            done[u] = 0;
            if (l[u] == -1) q.emplace(u), d[u] = 0;
            else d[u] = std::numeric_limits <int>::max();
        }

        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const auto &v : G[u]) {
                found |= r[v] == -1;
                if (~r[v] && d[u] + 1 < d[r[v]] ) {
                    d[r[v]] = d[u] + 1;
                    q.emplace(r[v]);
                }
            }
        }

        return found;
    }

    bool dfs(int u) {
        for (int &i = done[u]; i < (int)G[u].size(); ++i) {
            const int &v = G[u][i];
            if (r[v] == -1 || (d[r[v]] == d[u] + 1 && dfs(r[v]))) {
                l[u] = v; r[v] = u;
                return true;
            }
        }

        d[u] = std::numeric_limits <int>::max();
        return false;
    }

    int MaxMatching() {
        TLE::start();
        fill(l.begin(), l.end(), -1);
        fill(r.begin(), r.end(), -1);

        int res = 0;
        while (bfs()) {
            for (int u = 0; u < L; ++u) {
                if (l[u] == -1 && dfs(u)) ++res;
            }
            if (TLE::TLE()) return -1;
        }

        return res;
    }
};

/*
# Complxity: O(\sqrt{V}E) 

# Utility:
0th index for both parts
HopcroftKarp(L, R) -> number of vertices in the first and second parts, initializes algorithm
addEdge(u, v) -> adds an edge from u in the first part to v in the second part
MaxMatching() -> returns the size of the maximum matching

# References:
https://infoarena.ro/problema/cuplaj
https://codeforces.com/blog/entry/58048
https://epubs.siam.org/doi/10.1137/0202019
https://en.wikipedia.org/wiki/Hopcroft–Karp_algorithm
*/