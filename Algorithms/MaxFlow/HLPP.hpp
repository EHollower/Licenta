#pragma once

#include "../../Utility/clock.hpp"

#include <queue>
#include <string>
#include <vector>
#include <numeric>

struct HLPP {
    struct Edge {
        int dest, back, rem, cap;
    };

    int V, high, cut, work;
    std::vector <std::basic_string <Edge>> G;
    std::vector <long long> ex;
    std::vector <int> hi, curr, prv, nxt, act, bot;
    std::queue <int> q;

    HLPP(int V): V(V), G(V), ex(V), hi(V), curr(V), prv(V * 3), nxt(V * 3), act(V * 2), bot(V) {}
    inline void addEdge(int u, int v, int cap, int rcap = 0) {
        if (u == v) return;
        G[u].push_back({v, (int)G[v].size(), cap, cap});
        G[v].push_back({u, (int)G[u].size() - 1, rcap, rcap});
    }

    inline void updH(int v, int h) {
        prv[nxt[prv[v]] = nxt[v]] = prv[v];
        hi[v] = h;

        if (ex[v] > 0) {
            bot[v] = act[h], act[h] = v, high = std::max(high, h);
        }

        if (h < V) cut = std::max(cut, h + 1);

        nxt[v] = nxt[prv[v] = h += V];
        prv[nxt[nxt[h] = v]] = v;
    }

    inline void globalRelabel(int s, int t) {
        fill(hi.begin(), hi.end(), V * 2);
        fill(act.begin(), act.end(), -1);
        iota(prv.begin(), prv.end(), 0);
        iota(nxt.begin(), nxt.end(), 0);

        hi[t] = high = cut = work = 0;
        hi[s] = V;

        for (int x : {t, s}) {
            for (q.emplace(x); !q.empty(); q.pop()) {
                int v = q.front();
                for (auto &e : G[v])
                    if (hi[e.dest] == V * 2 && G[e.dest][e.back].rem) {
                        q.push(e.dest), updH(e.dest, hi[v] + 1);
                    }
            }
        }
    }

    inline void push(int v, Edge &e, bool z) {
        long long f = std::min <long long>(ex[v], e.rem);

        if (f > 0) {
            if (z && !ex[e.dest]) {
                bot[e.dest] = act[hi[e.dest]];
                act[hi[e.dest]] = e.dest;
            }

            e.rem -= f; G[e.dest][e.back].rem += f;
            ex[v] -= f; ex[e.dest] += f;
        }
    }

    inline void discharge(int v) {
        int h = V * 2, k = hi[v];
        for (int j = 0; j < (int)G[v].size(); ++j) {
            auto &e = G[v][curr[v]];

            if (e.rem) {
                if (k == hi[e.dest] + 1) {
                    push(v, e, true);

                    if (ex[v] <= 0) return;
                } else h = std::min(h, hi[e.dest] + 1);
            }

            if (++curr[v] >= (int)G[v].size()) curr[v] = 0;
        }

        if (k < V && nxt[k + V] == prv[k + V]) {
            for (int j = k; j < cut; ++j) {
                while (nxt[j + V] < V) updH(nxt[j + V], V);
            }
            cut = k;
        } else updH(v, h), ++work;
    }

   long long MaxFlow(int s, int t) {
        TLE::start();
        for (auto &e : G[s]) ex[s] = e.rem, push(s, e, 0);

        globalRelabel(s, t);

        for (; high; high--) {
            while (act[high] != -1) {
                int v = act[high];
                act[high] = bot[v];

                if (v != s && hi[v] == high) {
                    discharge(v);
                    if (work > 4 * V) globalRelabel(s, t);
                }
            }

            if (TLE::TLE()) return -1;
        }

        globalRelabel(s, t);
        return ex[t];
    }
};

/*
# Complexity:  O(V^2\sqrt E) - Gap + Highest Label + Global Relabeling

# Utility:
0th index verticies
HLPP(V) - constructor initalize with V vertices
addEdge(u, v, cap, rcap) - to add an edge from u to v with capacity cap and reverse capacity rcap
MaxFlow(s, t) - to calculate the maximum flow from source to sink

# References:
https://oi-wiki.org/graph/flow/max-flow/#hlpp-算法
https://codeforces.com/blog/entry/66006#comment-500868
https://link.springer.com/chapter/10.1007/978-3-540-87744-8_39
https://gist.github.com/Chillee/ad2110fc17af453fb6fc3357a78cfd28#file-hlpp-cpp
*/