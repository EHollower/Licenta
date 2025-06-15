#pragma once

#include "../../Utility/clock.hpp"

#include <list>
#include <queue>
#include <vector>
#include <limits>
#include <string>
#include <algorithm>

struct MPM {
    struct Edge {
        int back, dest, f, c;
    };

    int V, s, t;
    std::vector <Edge> edges;
    std::vector <bool> alive;
    std::vector <long long> pin, pout, ex;
    std::vector <std::list <int>> in, out;
    std::vector <std::basic_string <int>> G;
    std::vector <int> lvl;
    std::queue <int> q;

    MPM(int V): V(V), alive(V), pin(V), pout(V), ex(V), in(V), out(V), G(V), lvl(V) {}

    inline void addEdge(int u, int v, int cap, int rcap = 0) {
        if (u == v) return;
        edges.push_back({u, v, 0, cap});
        G[u].push_back((int)edges.size() - 1);
        edges.push_back({v, u, 0, rcap});
        G[v].push_back((int)edges.size() - 1);
    }

    bool bfs() {
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const auto &id : G[u]) {
                Edge &edge = edges[id];
                if (edge.c - edge.f  < 1) continue;
                if (lvl[edge.dest] != -1) continue;
                lvl[edge.dest] = lvl[u] + 1;
                q.emplace(edge.dest);
            }
        }
        return lvl[t] != -1;
    }

    long long pot(int u) {
        return std::min(pin[u], pout[u]);
    }

    void rmv_node(int u) {
        for (auto& i : in[u]) {
            const int &v = edges[i].back;
            out[v].remove(i);
            pout[v] -= (edges[i].c - edges[i].f);
        }

        for (const auto& i : out[u]) {
            const int &v = edges[i].dest;
            in[v].remove(i);
            pin[v] -= (edges[i].c - edges[i].f);
        }
    }

    void push(int back, int dest, long long flow, bool fwd) {
        std::queue <int> q;
        fill(ex.begin(), ex.end(), 0);

        ex[back] = flow; q.emplace(back);
        while (!q.empty()) {
            const int &u  = q.front(); q.pop();
            if (u == dest) break;

            long long must = ex[u];
            auto it = fwd ? out[u].begin() : in[u].begin();

            while (must > 0 && it != (fwd ? out[u].end() : in[u].end())) {
                const int &v = fwd ? edges[*it].dest : edges[*it].back;
                int avail  = edges[*it].c - edges[*it].f;
                long long pushed = std::min(must, 1LL * avail);
                if (pushed == 0) { ++it; break; }

                if (fwd) {
                    pout[u] -= pushed;
                    pin[v] -= pushed;
                } else {
                    pin[u] -= pushed;
                    pout[v] -= pushed;
                }

                if (ex[v] == 0) q.emplace(v);

                ex[v] += pushed;
                edges[*it].f += pushed;
                edges[*it ^ 1].f -= pushed;
                must -= pushed;

                if (pushed == avail) {
                    int idx = *it;
                    auto nxt = std::next(it);
                    if (fwd) {
                        in[v].remove(idx);
                        out[u].erase(it);
                    } else {
                        out[v].remove(idx);
                        in[u].erase(it);
                    }

                    it = nxt;
                } else break;
            }
        }
    }

    long long MaxFlow(int s, int t) {
        TLE::start();
        this->s = s; this->t = t;

        long long res = 0;

        while (true) {
            fill(pin.begin(), pin.end(), 0);
            fill(pout.begin(), pout.end(), 0);
            fill(lvl.begin(), lvl.end(), -1);
            fill(alive.begin(), alive.end(), true);

            lvl[s] = 0; q.emplace(s);

            if (!bfs()) break;

            for (int i = 0; i < V; ++i) in[i].clear(), out[i].clear();

            for (int i = 0; i < (int)edges.size(); ++i) {
                const auto &edge = edges[i];
                if (edge.c - edge.f == 0) continue;

                auto [u, v] = std::make_pair(edge.back, edge.dest);

                if (lvl[u] + 1 == lvl[v] && (lvl[v] < lvl[t] || v == t)) {
                    in[v].push_back(i); out[u].push_back(i);
                    pin[v]  += edge.c - edge.f; 
                    pout[u] += edge.c - edge.f;
                }
            }

            pin[s]  = std::numeric_limits <long long>::max();
            pout[t] = std::numeric_limits <long long>::max();
            
            while (true) {
                int u = -1;
                for (int i = 0; i < V; ++i) {
                    if (!alive[i]) continue;
                    if (u == -1 || pot(i) < pot(u)) u = i;
                }

                if (u == -1) break;

                if (pot(u) == 0) {
                    alive[u] = false;
                    rmv_node(u);
                    continue;
                }

                long long flow = pot(u);
                res += flow;
                push(u, s, flow, false);
                push(u, t, flow, true);
                alive[u] = false;
                rmv_node(u);

                if (TLE::TLE()) return -1;
            }
        }

        return res;
    }
};

/*
# Complexity: O(V^3)

# Utility:
0th index verticies
MPM(V) - constructor initialize with V vertices
addEdge(u, v, cap, rcap) - to add an edge from u to v with capacity cap and reverse capacity rcap
MaxFlow(s, t) - to calculate the maximum flow from source to sink

# References: 
https://cp-algorithms.com/graph/mpm.html
https://oi-wiki.org/graph/flow/max-flow/#mpm-算法
https://www.tr5.org/~ruijie/writings/MPM.pdf
*/