#pragma once

#include "../../Utility/clock.hpp"

#include <vector>
#include <limits>
#include <string>

struct PushRelabelHG {
    struct Edge {
        int dest, back, f, c;
    };

    int V;
    std::vector <std::basic_string <Edge>> G;
    std::vector <Edge*> curr; 
    std::vector <long long> exc;
    std::vector <std::vector <int>> hs;
    std::vector <int> h; 

    PushRelabelHG(int V): V(V), G(V), curr(V), exc(V), hs(2 * V), h(V) {}

    inline void addEdge(int u, int v, int cap, int rcap = 0) {
        if (u == v) return;
        Edge a = {v, (int)G[v].size(), 0, cap};
        Edge b = {u, (int)G[u].size(), 0, rcap};
        G[u].push_back(a);
        G[v].push_back(b);
    }

    inline void addFlow(Edge& e, long long f) {
        Edge& back = G[e.dest][e.back]; 
        if (!exc[e.dest] && f) hs[h[e.dest]].emplace_back(e.dest);
        e.f += f; e.c -= f; exc[e.dest] += f;
        back.f -= f; back.c += f; exc[back.dest] -= f; 
    }

    long long MaxFlow(int s, int t) {
        TLE::start();
        h[s] = V; exc[t] = 1;
        std::vector <int> seen(2 * V); seen[0] = V - 1;

        for (int i = 0; i < V; ++i) curr[i] = G[i].data();
        for (Edge &e : G[s]) addFlow(e, e.c);

        for (int hi = 0; ;) {
            while (hs[hi].empty()) if (!hi--) return -exc[s];
            int u = hs[hi].back(); hs[hi].pop_back();
            while (exc[u] > 0) { // discharge u
                if (curr[u] == G[u].data() + G[u].size()) {
                    h[u] = std::numeric_limits <int>::max();
                    for (Edge &e : G[u]) if(e.c && h[u] > h[e.dest] + 1) {
                        h[u] = h[e.dest] + 1;         
                        curr[u] = &e;
                    } 

                    if (++seen[h[u]], !--seen[hi] && hi < V) {
                        for (int i = 0; i < V; ++i) if (hi < h[i] && h[i] < V)
                            --seen[h[i]], h[i] = V + 1;
                    }

                    hi = h[u];
                } else if (curr[u] -> c && h[u] == h[curr[u]-> dest] + 1) {
                    addFlow(*curr[u], std::min(exc[u], 1LL * curr[u]-> c)); 
                } else ++curr[u];
            }

            if (TLE::TLE()) return -1;
        }
    }
};

/*
# Complexity:  O(V^2\sqrt E) - Gap + Highest Label

# Utility:
0th index verticies
PushRelabel(V) - constructor initalize with V vertices
addEdge(u, v, cap, rcap) - to add an edge from u to v with capacity cap and reverse capacity rcap
MaxFlow(s, t) - to calculate the maximum flow from source to sink

# References:
https://epubs.siam.org/doi/10.1137/0218072
https://cp-algorithms.com/graph/push-relabel-faster.html
https://oi-wiki.org/graph/flow/max-flow/#push-relabel-预流推进算法
https://github.com/kth-competitive-programming/kactl/blob/main/content/graph/PushRelabel.h
*/