#include <filesystem>
#include <iostream>
#include <cassert>
#include <string>

#include "../../Algorithms/MaxFlow/allMaxFlow.hpp"

void Open(const std::filesystem::path& path) {
    const std::string path_str = "testCases\\" + path.string();
    (void)!freopen(path_str.c_str(), "r", stdin);
}

void Close() {
    (void)!fclose(stdin);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    namespace fs = std::filesystem;
    const fs::path test_dir = "testCases";

    int failures = 0;
    for (const auto &entry : fs::directory_iterator(test_dir)) {
        Open(entry.path().filename());

        int V, E, s, t; std::cin >> V >> E >> s >> t;

        HLPP alg2(V);
        PushRelabelHG alg(V);

        assert(V > 0 && E >= 0);
        assert(s >= 0 && s < V);
        assert(t >= 0 && t < V);
        for (int i = 0; i < E; ++i) {
            int u, v, cap; std::cin >> u >> v >> cap;
            assert(u >= 0 && u < V);
            assert(v >= 0 && v < V);
            assert(cap >= 0);
            alg.addEdge(u, v, cap);
            alg2.addEdge(u, v, cap);
        }

        long long flow = alg.MaxFlow(s, t);
        long long flow2 = alg2.MaxFlow(s, t);
        assert(flow == flow2);
        Close();

        if (flow < 0) {
            std::cerr << "Test " << entry.path().filename() << " failed: flow=" << flow << std::endl;
            return 1;
        } 

        std::cout << "Test " << entry.path().filename() << " passed: flow=" << flow << std::endl;
        std::cerr << V << " " << E << " " << s << " " << t << std::endl;
    }

    return 0;
}

