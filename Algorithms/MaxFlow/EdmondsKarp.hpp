#pragma once

#include "../../Utility/clock.hpp"

#include <queue>
#include <vector>
#include <limits>
#include <string>

struct EdmondsKarp {
    struct Edge {
        int back, dest, f, c;
    };

    std::vector <std::basic_string <int>> G; 
    std::vector <Edge> edg;
    std::vector <int> p, a; 

    EdmondsKarp(int V) : G(V), p(V), a(V) {}

    inline void addEdge(int u, int v, int cap, int rcap = 0) {
        if (u == v) return;
        edg.push_back({u, v, 0, cap});
        G[u].push_back((int)edg.size() - 1);
        edg.push_back({v, u, 0, rcap});
        G[v].push_back((int)edg.size() - 1);
    }

    long long MaxFlow(int s, int t) {
        TLE::start();
        long long flow = 0;
        while (true) {
           fill(a.begin(), a.end(), 0);
           a[s] = std::numeric_limits <int>::max();

           std::queue <int> q; q.emplace(s);
           while (!q.empty()) {
                int u = q.front(); 
                q.pop();

                for (int idx : G[u]) {
                    auto &e = edg[idx];
                    if (!a[e.dest] && e.c > e.f) {
                        p[e.dest] = idx;
                        a[e.dest] = std::min(a[u], e.c - e.f);
                        q.emplace(e.dest);
                    }
                }
                if (a[t]) break;
            }

            if (!a[t]) break;

            for (int u = t; u != s; u = edg[p[u]].back) {
                edg[p[u]].f += a[t];
                edg[p[u] ^ 1].f -= a[t];
            }

            flow += a[t];
            if (TLE::TLE()) return -1;
        }

        return flow;
    }
};

/*
# Complexity: O(VE^2)

# Utility:
0th index verticies
EdmondsKarp(V) - constructor initialize with V vertices
addEdge(u, v, cap, rcap) - to add an edge with capacity and reverse capacity
MaxFlow(s, t) - to get the maximum flow from s to t

# References:
https://www.youtube.com/watch?v=RppuJYwlcI8
https://cp-algorithms.com/graph/edmonds_karp.html
https://en.wikipedia.org/wiki/Edmonds–Karp_algorithm
https://en.oi-wiki.org/graph/flow/max-flow/#ford-fulkerson
https://web.eecs.umich.edu/~pettie/matching/Edmonds-Karp-network-flow.pdf
*/
