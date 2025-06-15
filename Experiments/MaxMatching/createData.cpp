#include <string>
#include <future>
#include <cassert>
#include <fstream>
#include <iostream>
#include <filesystem>

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
    std::ofstream output("maxmatching.csv");

    output << "Testcase,Algorithm,Nodes,Edges,MaxMatching,RuntimeMs" << std::endl;

    for (const auto &entry : fs::directory_iterator(test_dir)) {
        const std::string filename = entry.path().filename().string();
        Open(entry.path().filename());

        int L, R, E; std::cin >> L >> R >> E;

        struct Edge { int u, v; };
        std::vector <Edge> edges(E);
        for (auto &[u, v] : edges) std::cin >> u >> v;

        auto run = [&](auto &&G, const std::string &name) {
            for (const Edge &e : edges) G.addEdge(e.u, e.v);

            auto start = std::chrono::high_resolution_clock::now();
            long long matching = G.MaxMatching();

            if (matching < 0) {
                output << filename << ',' << name << ',' << L + R << ',' << E << ",TLE,TLE" << std::endl;
                return;
            }

            auto end = std::chrono::high_resolution_clock::now();
            long long duration = std::chrono::duration_cast <std::chrono::milliseconds>(end - start).count();
            output << filename << ',' << name << ',' << L + R << ',' << E << ',' << matching << ',' << duration << std::endl;
        };

        auto runFlow = [&](auto &&G, const std::string &name) {
            int s = L + R, t = L + R + 1;
            for (const Edge &e : edges) G.addEdge(e.u, e.v + L, 1);
            for (int i = 0; i < L; ++i) G.addEdge(s, i, 1);
            for (int i = L; i < L + R; ++i) G.addEdge(i, t, 1);

            auto start = std::chrono::high_resolution_clock::now();
            long long flow = G.MaxFlow(s, t);

            if (flow < 0) {
                output << filename << ',' << name << ',' << L + R << ',' << E << ",TLE,TLE" << std::endl;
                return;
            }

            auto end = std::chrono::high_resolution_clock::now();
            long long duration = std::chrono::duration_cast <std::chrono::milliseconds>(end - start).count();
            output << filename << ',' << name << ',' << L + R << ',' << E << ',' << flow << ',' << duration << std::endl;
        };


        // Khun
        run(Khun(L, R), "Khun");
        run(KhunArb(L, R), "Khun-Arb");
        run(KhunRnd(L, R), "Khun-Rnd");

        // Dinic
        runFlow(Dinic(L + R + 2), "Dinic");
        runFlow(DinicScaling(L + R + 2), "Dinic-Scaling");

        // Hopcroft-Karp
        run(HopcroftKarp(L, R), "Hopcroft-Karp");
        run(HopcroftKarpLin(L, R), "Hopcroft-Karp-Lin");

        std::cerr << "Test " << entry.path().filename() << " complete" << std::endl;
        Close();
    }

    output.close();
    return 0;
}