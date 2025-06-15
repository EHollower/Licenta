#pragma once

#include "../../Utility/clock.hpp"

#include <queue>
#include <vector>
#include <limits>
#include <string>

struct ISAP {
    struct Edge {
        int back, dest, f, c;
    };

    int V, s, t;
    std::vector <Edge> edges;
    std::vector <std::basic_string <int>> G;
    std::vector <int> d, curr, p, num;
    std::vector <bool> vis;

    ISAP(int V) : V(V), G(V), d(V), curr(V), p(V), num(V + 5), vis(V) {}

    inline void addEdge(int u, int v, int cap, int rcap = 0) {
        if (u == v) return;
        edges.push_back({u, v, 0, cap});
        G[u].push_back((int)edges.size() - 1);
        edges.push_back({v, u, 0, rcap});
        G[v].push_back((int)edges.size() - 1);
    }

    inline bool bfs() {
        fill(vis.begin(), vis.end(), false);
        vis[t] = true; d[t] = 0;
        std::queue <int> q; q.emplace(t);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const int &idx : G[u]) {
                Edge &e = edges[idx ^ 1];
                if (!vis[e.back] && e.c - e.f > 0) {
                    vis[e.back] = true;
                    d[e.back] = d[u] + 1;
                    q.emplace(e.back);
                }
            }
        }

        return vis[s];
    }

    inline int augment() {
        int u = t, flow = std::numeric_limits <int>::max();
        while (u != s) {
            Edge &e = edges[p[u]];
            flow = std::min(flow, e.c - e.f);
            u = edges[p[u]].back;
        }

        u = t;
        while (u != s) {
            edges[p[u]].f += flow;
            edges[p[u] ^ 1].f -= flow;
            u = edges[p[u]].back;
        }

        return flow;
    }

    long long MaxFlow(int s, int t) {
        TLE::start();
        this-> s = s, this-> t = t;
        long long flow = 0;

        bfs();
        fill(num.begin(), num.end(), 0);
        for (int i = 0; i < V; ++i) ++num[d[i]];

        int u = s;
        fill(curr.begin(), curr.end(), 0);
        
        while (d[s] < V) {
            if (u == t) {
                flow += augment();
                u = s;
            }

            bool ok = false;
            for (int i = curr[u]; i < (int)G[u].size(); ++i) {
                Edge &e = edges[G[u][i]];
                if (d[u] == d[e.dest] + 1 && e.c - e.f > 0) {
                    ok = true;
                    p[e.dest] = G[u][i];
                    curr[u] = i;
                    u = e.dest;
                    break;
                }
            }

            if (!ok) {
                int m = V - 1;
                for (int i = 0; i < (int)G[u].size(); ++i) {
                    Edge &e = edges[G[u][i]];
                    if (e.c - e.f > 0) m = std::min(m, d[e.dest]);
                }

                if (--num[d[u]] == 0) break;
                ++num[d[u] = m + 1];
                curr[u] = 0;
                if (u != s) u = edges[p[u]].back;

            }

            if (TLE::TLE()) return -1;
        }

        return flow;
    }
};

/*
# Complexity: O(V^2 \times E)

# Utility:
0th index verticies
SAP(V) - constructor init. with V vertices
addEdge(u, v, cap, rcap) - to add an edge with capacity and reverse capacity
MaxFlow(s, t) - to get the maximum flow from s to t

# References
https://www.npmjs.com/package/@algorithm.ts/isap
https://en.oi-wiki.org/graph/flow/max-flow/#isap
*/