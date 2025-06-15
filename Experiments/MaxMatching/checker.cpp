#include <filesystem>
#include <iostream>
#include <cassert>
#include <string>

#include "../../Algorithms/MaxMatching/allMaxMatching.hpp"

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

        int L, R, E; std::cin >> L >> R >> E;
        assert(L > 0 && R > 0 && E >= 0);

        HopcroftKarp alg(L, R);
        HopcroftKarpLin alg0(L, R);
        for (int i = 0; i < E; ++i) {
            int u, v; std::cin >> u >> v;
            assert(u >= 0 && u < L);
            assert(v >= 0 && v < R);
            alg.addEdge(u, v);
            alg0.addEdge(u, v);
        }

        int matching = alg.MaxMatching();
        int matching0 = alg0.MaxMatching();

        assert(matching == matching0);

        if (matching < 0) {
            std::cerr << "Test " << entry.path().filename() << " failed: matching=" << matching << std::endl;
            return 1;
        }

        std::cout << "Test " << entry.path().filename() << " passed: matching=" << matching << std::endl;
    }

    return 0;
}