#pragma once

#include "../../Utility/clock.hpp"

#include <vector>
#include <string>

struct Scaling {
    struct Edge {
        int back, dest, w;
    };

    int V, s, t;
    std::vector <Edge> edges;
    std::vector <bool> seen;
    std::vector <std::basic_string <int>> G;

    Scaling(int V): V(V), seen(V), G(V) {}

    inline void addEdge(int u, int v, int cap, int rcap = 0) {
        if (u == v) return;
        edges.push_back({u, v, cap});
        G[u].push_back((int)edges.size() - 1);
        edges.push_back({v, u, rcap});
        G[v].push_back((int)edges.size() - 1);
    }

    inline bool dfs(int u, int flow) {
        if (u == t) return true;
        if (seen[u]) return false;

        seen[u] = true;
        for (const auto& id : G[u]) {
            Edge &edge = edges[id], &back = edges[id ^ 1];

            if (edge.w - flow < 0) continue;

            if (dfs(edge.dest, flow)) {
                edge.w -= flow;
                back.w += flow;
                return true;
            }
        }

        return false;
    }

    long long MaxFlow(int s, int t) {
        TLE::start();
        this-> s = s, this-> t = t;

        long long res = 0;
        for (int bit = 1 << 30; bit > 0; bit >>= 1) {
            bool found = true;
            do {
                fill(seen.begin(), seen.end(), false);
                found = dfs(s, bit);
                res += bit * found;

                if (TLE::TLE()) return -1;

            } while (found);
        }
        return res;
    }
};

/*
# Complexity: O(E^2 log U)

# Utility:
0th index verticies
Scaling(V) - constructor init. with V vertices
addEdge(u, v, cap, rcap) - to add an edge with capacity and reverse capacity
MaxFlow(s, t) - to get the maximum flow from s to t

# References
https://cses.fi/book/book.pdf
https://codeforces.com/blog/entry/52714
https://www.sciencedirect.com/science/article/pii/002200008590039X
*/