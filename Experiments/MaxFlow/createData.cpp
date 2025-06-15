#include <string>
#include <future>
#include <cassert>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "../../Algorithms/MaxFlow/AllMaxFlow.hpp"

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
    std::ofstream output("maxflow.csv");

    output << "Testcase,Algorithm,Nodes,Edges,MaxFlow,RuntimeMs" << std::endl;

    for (const auto &entry : fs::directory_iterator(test_dir)) {
        const std::string filename = entry.path().filename().string();
        Open(entry.path().filename());
        std::cin.clear();

        int n, m, s, t; std::cin >> n >> m >> s >> t;

        struct Edge { int u, v, cap; };
        std::vector <Edge> edges(m);
        for (auto &[u, v, cap] : edges) std::cin >> u >> v >> cap;

        auto run = [&](auto &&G, const std::string &name) {
            for (const Edge &e : edges) G.addEdge(e.u, e.v, e.cap);

            auto start = std::chrono::high_resolution_clock::now();
            long long flow = G.MaxFlow(s, t);

            if (flow < 0) {
                output << filename << ',' << name << ',' << n << ',' << m << ",TLE,TLE" << std::endl;
                return;
            }

            auto end = std::chrono::high_resolution_clock::now();
            long long duration = std::chrono::duration_cast <std::chrono::milliseconds>(end - start).count();
            output << filename << ',' << name << ',' << n << ',' << m << ',' << flow << ',' << duration << std::endl;
        };

        // Augmented path
        run(EdmondsKarp(n), "Edmonds-Karp");
        run(Scaling(n), "Scaling");
        run(Dinic(n), "Dinic");
        run(DinicScaling(n), "Dinic-Scaling");
        run(ISAP(n), "ISAP");
        run(MPM(n), "MPM");

        // Preflow
        run(PushRelabel(n), "Push-Relabel");
        run(PushRelabelHighest(n), "Push-Relabel-Highest");
        run(PushRelabelGap(n), "Push-Relabel-Gap");
        run(PushRelabelHG(n), "Push-Relabel-HG");
        run(HLPP(n), "HLPP");
        
        std::cerr << "Test " << entry.path().filename() << " complete" << std::endl;
        Close();
    }

    output.close();
    return 0;
}