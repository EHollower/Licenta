// https://codeforces.com/blog/entry/58048?#comment-417533
// https://github.com/yosupo06/library-checker-problems/blob/master/graph/bipartitematching/gen/kuhn_killer.cpp
#pragma once

#include <vector>
#include <utility>
#include <iostream>

void genKhunKiller(int L, int R, const long long &seed, const std::string &filename) {
    int n = std::min(L, R);
    int k = n / 8 * 2;

    using P = std::pair <int, int>;
    std::vector <P> edges;
    for(int i = 0; i < k; i++) {
        edges.emplace_back(i, i);
    }

    for(int i = 0; i < k - 1; i++) {
        edges.emplace_back(i + 1, i);
    }

    for(int i = 0; i < k / 2; i++) {
        edges.emplace_back(i, k + 2 * i);
        edges.emplace_back(2 * k + 2 * i, k + 2 * i);
        edges.emplace_back(2 * k + 2 * i, 3 * k + 2 * i);
        edges.emplace_back(k + 2 * i, i);
        edges.emplace_back(k + 2 * i, 2 * k + 2 * i);
        edges.emplace_back(3 * k + 2 * i, 2 * k + 2 * i);
    }
    
    for(int j = 0; j < k / 2; j++) {
        int i = k / 2 - 1 - j;
        edges.emplace_back(k / 2 + j, k + 2 * i + 1);
        edges.emplace_back(2 * k + 2 * i + 1, k + 2 * i + 1);
        edges.emplace_back(2 * k + 2 * i + 1, 3 * k + 2 * i + 1);
        edges.emplace_back(k + 2 * i + 1, k / 2 + j);
        edges.emplace_back(k + 2 * i + 1, 2 * k + 2 * i + 1);
        edges.emplace_back(3 * k + 2 * i + 1, 2 * k + 2 * i + 1);
    }
    
    int m = int(edges.size());

    (void)!freopen(filename.c_str(), "w", stdout);

    std::cout << n << ' ' << n << ' ' << m << '\n';
    for (const auto& [a, b] : edges) std::cout << a << ' ' << b << '\n';

}
