#pragma once

#include "../../Utility/clock.hpp"

#include <vector>
#include <limits>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>

struct HopcroftKarpLin {
    int L;
    std::vector <std::basic_string <int>> G;
    std::vector <int> l, r, p, a;
    std::mt19937 rnd;

    HopcroftKarpLin(int L, int R) : L(L), G(L), l(L), r(R), p(L), a(L) {
        rnd.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    }

    inline void addEdge(int u, int v) { G[u].push_back(v); }

    int MaxMatching() {
        for (int i = 0; i < L; ++i) {
            shuffle(G[i].begin(), G[i].end(), rnd);
        }

        fill(l.begin(), l.end(), -1);
        fill(r.begin(), r.end(), -1);

        std::vector <int> q(L);

        bool match = true;
        while (match) {
            fill(a.begin(), a.end(), -1);
            fill(p.begin(), p.end(), -1);

            int t = 0;
            for (int i = 0; i < L; ++i)
                if (l[i] == -1) q[t++] = a[i] = p[i] = i;

            match = false;
            for (int i = 0; i < t; ++i) {
                int u = q[i];
                if (~l[a[u]]) continue;

                for (int v : G[u]) {
                    if (r[v] == -1) {
                        while (~v) r[v] = u, std::swap(l[u], v), u = p[u];
                        match = true;
                        break;
                    } 
                    
                    if (p[r[v]] == -1) q[t++] = v = r[v], p[v] = u, a[v] = a[u];
                }
            }
        }

        int res = 0;
        for (int i = 0; i < L; ++i) res += (l[i] != -1);
        return res;
    }
};