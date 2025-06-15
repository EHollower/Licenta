#pragma once

#include "../../Utility/clock.hpp"

#include <queue>
#include <vector>
#include <limits>
#include <string>

struct DinicScaling {
    struct Edge {
        int back, dest, f, c;
    };

    int s, t;
    std::vector <std::basic_string <Edge>> G;
    std::vector <int> d, done;

    DinicScaling(int V) : G(V), d(V), done(V) {}

    inline void addEdge(int u, int v, int cap, int rcap = 0) { 
        if (u == v) return; 
        G[u].push_back({(int)G[v].size(), v, 0, cap});
        G[v].push_back({(int)G[u].size() - 1, u, 0, rcap});
    }

    bool bfs() {
        fill(d.begin(), d.end(), -1);

        d[s] = 0;
        std::queue <int> q; q.emplace(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const Edge &e : G[u]) {
                if (d[e.dest] == -1 && e.c - e.f > 0) {
                    d[e.dest] = d[u] + 1;
                    q.emplace(e.dest);
                }
            }
        }

        return d[t] != -1;
    }

    int dfs(int u, int flow) {
        if (u == t) return flow;

        for (auto &i = done[u]; i < (int)G[u].size(); ++i) {
            Edge &e = G[u][i];
            if (d[e.dest] == d[u] + 1 && e.c - e.f > 0) {
                int pushed = dfs(e.dest, std::min(flow, e.c - e.f));
                if (pushed > 0) {
                    e.f += pushed;
                    G[e.dest][e.back].f -= pushed;
                    return pushed;
                }
            }
        }

        return 0;
    }

    long long MaxFlow(int s, int t) {
        TLE::start();
        this-> s = s, this-> t = t;

        long long flow = 0;
        for (int bit = 1 << 30; bit > 0; bit >>= 1) {
            while (bfs()) {
                fill(done.begin(), done.end(), 0);
                while (int pushed = dfs(s, bit)) {
                    flow += pushed;
                    if (TLE::TLE()) return -1;
                }
            }
        }

        return flow;
    }
};

/*
# Complexity: O(VE log(U))
O(\min(E^{1/2}, V^{2/3})E) if U = 1

# Utility:
0th index verticies
DinicScaling(V) - constructor initialize network with V vertices
addEdge(u, v, cap, rcap) - to add an edge with capacity and reverse capacity
MaxFlow(s, t) - to get the maximum flow from s to t

# References:
https://codeforces.com/blog/entry/66006
https://codeforces.com/blog/entry/52714
https://www.sciencedirect.com/science/article/pii/002200008590039X
https://github.com/kth-competitive-programming/kactl/blob/main/content/graph/Dinic.h
*/