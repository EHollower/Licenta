#include "../../Utility/clock.hpp"

#include <queue>
#include <vector>
#include <limits>
#include <string>

struct PushRelabelGap {
    struct Edge {
        int back, dest, f, c;
    };

    int V;
    std::vector <std::basic_string <int>> G;
    std::vector <Edge> edg;
    std::vector <int> h, cnt;
    std::vector <bool> active;
    std::vector <long long> ex;
    std::queue <int> excess_v;

    PushRelabelGap(int V) : V(V), G(V), h(V), cnt(2 * V), active(V), ex(V) {}

    inline void addEdge(int u, int v, int cap, int rcap = 0) {
        if (u == v) return;
        edg.push_back({u, v, 0, cap});
        G[u].push_back((int)edg.size() - 1);
        edg.push_back({v, u, 0, rcap});
        G[v].push_back((int)edg.size() - 1);
    }

    inline void enqueue(int u) {
        if (!active[u] && ex[u] > 0) {
            active[u] = true;
            excess_v.emplace(u);
        }
    }

    inline void push(int idx) {
        auto [u, dest] = std::make_pair(edg[idx].back, edg[idx].dest);

        int avail = edg[idx].c - edg[idx].f;
        int d = ex[u] < avail ? ex[u] : avail;

        if (d <= 0 || h[u] <= h[dest]) return;

        edg[idx].f += d, edg[idx ^ 1].f -= d;
        ex[u] -= d; ex[dest] += d;
        enqueue(dest);
    }

    inline void gap(int k) {
        for (int u = 0; u < V; ++u) {
            if (h[u] > k) {
                --cnt[h[u]];
                h[u] = std::max(h[u], V + 1);
                ++cnt[h[u]];
                enqueue(u);
            }
        }
    }

    inline void relabel(int u) {
        --cnt[h[u]];
        h[u] = 2 * V;
        for (const int &idx : G[u]) {
            Edge &e = edg[idx];
            if (e.c - e.f > 0) h[u] = std::min(h[u], h[e.dest] + 1);
        }
        ++cnt[h[u]];
        enqueue(u);
    }

    inline void discharge(int u) {
        for (auto& idx : G[u]) {
            if (ex[u] <= 0) break;
            push(idx);
        }

        if (ex[u] > 0) {
            if (cnt[h[u]] == 0) gap(h[u]);
            else relabel(u);
        }
    }

    long long MaxFlow(int s, int t) {
        TLE::start();

        h[s] = V;
        cnt[0] = V - 1; cnt[V] = 1;
        active[s] = active[t] = true;

        for (const auto &idx : G[s]) {
            ex[s] += edg[idx].c;
            push(idx);
        }

        while (!excess_v.empty()) {
            int u = excess_v.front(); excess_v.pop();
            active[u] = false;

            discharge(u);
            
            if (TLE::TLE()) return -1;
        }

        return ex[t];
    }
};

/*
# Complexity: O(V^3) worst case

# Utility:
0th index verticies
PushRelabelHighest(V) - constructor initialize with V vertices  
addEdge(u, v, cap, rcap) - to add an edge from u to v with capacity cap and reverse capacity rcap
MaxFlow(s, t) - to calculate the maximum flow from source to sink

# References:
https://codeforces.com/blog/entry/68494
https://epubs.siam.org/doi/10.1137/0218072
https://cp-algorithms.com/graph/push-relabel-faster.html
https://en.wikipedia.org/wiki/Push-relabel_maximum_flow_algorithm
http://i.stanford.edu/pub/cstr/reports/cs/tr/94/1523/CS-TR-94-1523.pdf
*/