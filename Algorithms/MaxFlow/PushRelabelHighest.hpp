#pragma once

#include "../../Utility/clock.hpp"

#include <queue>
#include <vector>
#include <limits>
#include <string>

struct PushRelabelHighest {
    struct Edge {
        int back, dest, f, c;
    };

    int V, s, t;
    std::vector <std::basic_string <int>> G;
    std::vector <std::vector <int>> hs;
    std::vector <Edge> edg;
    std::vector <int> h;
    std::vector <long long> ex;

    PushRelabelHighest(int V) : V(V), G(V), hs(2 * V), h(V), ex(V) {}

    inline void addEdge(int u, int v, int cap, int rcap = 0) {
        if (u == v) return;
        edg.push_back({u, v, 0, cap});
        G[u].push_back((int)edg.size() - 1);
        edg.push_back({v, u, 0, rcap});
        G[v].push_back((int)edg.size() - 1);
    }

    inline void push(int idx) {
        auto [u, dest] = std::make_pair(edg[idx].back, edg[idx].dest);

        int avail = edg[idx].c - edg[idx].f;
        int d = ex[u] < avail ? ex[u] : avail;

        if (dest != s && dest != t && !ex[dest] && d) hs[h[dest]].emplace_back(dest);

        edg[idx].f += d, edg[idx ^ 1].f -= d;
        ex[u] -= d; ex[dest] += d;
    }

    inline int relabel(int u) {
        h[u] = 2 * V;
        for (const int &idx : G[u]) {
            Edge &e = edg[idx];
            if (e.c - e.f > 0) h[u] = std::min(h[u], h[e.dest] + 1);
        }

        hs[h[u]].emplace_back(u);
        return h[u];
    }

    long long MaxFlow(int s, int t) {
        TLE::start();
        this->s = s, this->t = t;

        h[s] = V;
        ex[s] = std::numeric_limits <long long>::max();
        for (const auto &idx : G[s]) push(idx);
 
        for (int hi = 0; ; ) {
            while (hs[hi].empty()) if (!hi--) return ex[t];

            int u = hs[hi].back(); hs[hi].pop_back();
            while (ex[u] > 0) {
                for (const auto &idx : G[u]) {
                    if (!ex[u]) break;
                    if (edg[idx].c - edg[idx].f > 0 && h[u] == h[edg[idx].dest] + 1) 
                        push(idx);
                }
 
                if (ex[u]) hi = relabel(u);
            }

            if (TLE::TLE()) return -1;
        }
    }
};
/*
# Complexity: O(V^3) worst case

# Utility:
0th index verticies
PushRelabelHighest(V) - constructor init. with V vertices
addEdge(u, v, cap, rcap) - to add an edge from u to v with capacity cap and reverse capacity rcap
MaxFlow(s, t) - to calculate the maximum flow from source to sink

# References:
https://codeforces.com/blog/entry/68494
https://epubs.siam.org/doi/10.1137/0218072
https://cp-algorithms.com/graph/push-relabel-faster.html
https://en.wikipedia.org/wiki/Push-relabel_maximum_flow_algorithm
*/
