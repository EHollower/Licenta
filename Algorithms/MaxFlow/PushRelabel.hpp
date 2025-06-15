#pragma once

#include "../../Utility/clock.hpp"

#include <queue>
#include <vector>
#include <limits>
#include <string>

struct PushRelabel {
    struct Edge {
        int back, dest, f, c;
    };

    int V;
    std::vector <std::basic_string <int>> G;
    std::vector <Edge> edg;
    std::queue <int> excess_v;
    std::vector <int> height, seen;
    std::vector <long long> excess;

    PushRelabel(int V) : V(V), G(V), height(V), seen(V), excess(V) {}

    inline void addEdge(int u, int v, int cap, int rcap = 0) {
        if (u == v) return;
        edg.push_back({u, v, 0, cap});
        G[u].push_back((int)edg.size() - 1);
        edg.push_back({v, u, 0, rcap});
        G[v].push_back((int)edg.size() - 1);
    }

    inline void push(int idx) {
        auto [u, v] = std::make_pair(edg[idx].back, edg[idx].dest);

        int avail = edg[idx].c - edg[idx].f;
        int d = excess[u] < avail ? excess[u] : avail;

        if (d <= 0) return;

        edg[idx].f += d; 
        edg[idx ^ 1].f -= d;
        excess[u] -= d; excess[v] += d;
        if (d && excess[v] == d) {
            excess_v.push(v);
        }
    }

    inline void relabel(int u) {
        int min_h = std::numeric_limits <int>::max();
        for (const int &idx : G[u]) {
            Edge &e = edg[idx];
            if (e.c - e.f > 0) {
                min_h = std::min(min_h, height[e.dest]);
            }
        }

        if (min_h < std::numeric_limits <int>::max()) {
            height[u] = min_h + 1;
        }
    }

    inline void discharge(int u) {
        while (excess[u] > 0) {
            if (seen[u] < (int)G[u].size()) {
                int idx = G[u][seen[u]];
                Edge &e = edg[idx];
                
                if (e.c - e.f > 0 && height[u] > height[e.dest]) push(idx);
                else ++seen[u];
            } else {
                relabel(u);
                seen[u] = 0;
            }
        }
    }

    long long MaxFlow(int s, int t) {
        TLE::start();
        height[s] = V;
        excess[s] = std::numeric_limits <long long>::max();

        for (const int &idx : G[s]) push(idx);

        while (!excess_v.empty()) {
            int u = excess_v.front();
            excess_v.pop();
            if (u != s && u != t) discharge(u);
            if (TLE::TLE()) return -1;
        }

        return excess[t];
    }
};

/*
# Complexity: O(V^3) - FIFO

# Utility:
0th index verticies
PushRelabel(V) - constructor initialize with V vertices
addEdge(u, v, cap, rcap) - to add an edge from u to v with capacity cap and reverse capacity rcap
MaxFlow(s, t) - to calculate the maximum flow from source to sink

# References:
https://dl.acm.org/doi/pdf/10.1145/12130.12144
https://cp-algorithms.com/graph/push-relabel.html
https://en.oi-wiki.org/graph/flow/max-flow/#push-relabel
https://en.wikipedia.org/wiki/Push-relabel_maximum_flow_algorithm
*/