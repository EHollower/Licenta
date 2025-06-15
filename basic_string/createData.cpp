#include <string>
#include <future>
#include <cassert>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <queue>
#include <vector>
#include <limits>
#include <string>
#include <iterator>

void Open(const std::filesystem::path& path) {
    const std::string path_str = "testCases\\" + path.string();
    (void)!freopen(path_str.c_str(), "r", stdin);
}

void Close() {
    (void)!fclose(stdin);
}

struct HopcroftKarp0 {
    int L;
    std::vector <std::vector <int>> G;
    std::vector <int> l, r, d, done;

    HopcroftKarp0(int L, int R) : L(L), G(L), l(L), r(R), d(L), done(L) {}

    inline void addEdge(int u, int v) { G[u].push_back(v); }

    bool bfs() {
        std::queue <int> q;
        for (int u = 0; u < L; ++u) {
            done[u] = 0;
            if (l[u] == -1) q.emplace(u), d[u] = 0;
            else d[u] = std::numeric_limits <int>::max();
        }

        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const auto &v : G[u]) {
                found |= r[v] == -1;
                if (~r[v] && d[u] + 1 < d[r[v]] ) {
                    d[r[v]] = d[u] + 1;
                    q.emplace(r[v]);
                }
            }
        }

        return found;
    }

    bool dfs(int u) {
        for (int &i = done[u]; i < (int)G[u].size(); ++i) {
            const int &v = G[u][i];
            if (r[v] == -1 || (d[r[v]] == d[u] + 1 && dfs(r[v]))) {
                l[u] = v; r[v] = u;
                return true;
            }
        }

        d[u] = std::numeric_limits <int>::max();
        return false;
    }

    int MaxMatching() {
        fill(l.begin(), l.end(), -1);
        fill(r.begin(), r.end(), -1);

        int res = 0;
        while (bfs()) {
            for (int u = 0; u < L; ++u) {
                if (l[u] == -1 && dfs(u)) ++res;
            }
        }

        return res;
    }
};

struct HopcroftKarp1 {
    int L;
    std::vector <std::basic_string <int>> G;
    std::vector <int> l, r, d, done;

    HopcroftKarp1(int L, int R) : L(L), G(L), l(L), r(R), d(L), done(L) {}

    inline void addEdge(int u, int v) { G[u].push_back(v); }

    bool bfs() {
        std::queue <int> q;
        for (int u = 0; u < L; ++u) {
            done[u] = 0;
            if (l[u] == -1) q.emplace(u), d[u] = 0;
            else d[u] = std::numeric_limits <int>::max();
        }

        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const auto &v : G[u]) {
                found |= r[v] == -1;
                if (~r[v] && d[u] + 1 < d[r[v]] ) {
                    d[r[v]] = d[u] + 1;
                    q.emplace(r[v]);
                }
            }
        }

        return found;
    }

    bool dfs(int u) {
        for (int &i = done[u]; i < (int)G[u].size(); ++i) {
            const int &v = G[u][i];
            if (r[v] == -1 || (d[r[v]] == d[u] + 1 && dfs(r[v]))) {
                l[u] = v; r[v] = u;
                return true;
            }
        }

        d[u] = std::numeric_limits <int>::max();
        return false;
    }

    int MaxMatching() {
        fill(l.begin(), l.end(), -1);
        fill(r.begin(), r.end(), -1);

        int res = 0;
        while (bfs()) {
            for (int u = 0; u < L; ++u) {
                if (l[u] == -1 && dfs(u)) ++res;
            }
        }

        return res;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    namespace fs = std::filesystem;
    const fs::path test_dir = "testCases";
    std::ofstream output("basicstring.csv");

    output << "Testcase,Algorithm,Nodes,Edges,Matching,RuntimeMs" << std::endl;

    for (const auto &entry : fs::directory_iterator(test_dir)) {
        const std::string filename = entry.path().filename().string();
        Open(entry.path().filename());

        int L, R, E; std::cin >> L >> R >> E;

        struct Edge { int u, v; };
        std::vector <Edge> edges(E);
        for (auto &[u, v] : edges) std::cin >> u >> v;

        auto run = [&](auto &&G, const std::string &name) {
            auto start = std::chrono::high_resolution_clock::now();
            for (const Edge &e : edges) G.addEdge(e.u, e.v);

            int match = G.MaxMatching();

            auto end = std::chrono::high_resolution_clock::now();
            long long duration = std::chrono::duration_cast <std::chrono::milliseconds>(end - start).count();
            output << filename << ',' << name << ',' << L + R << ',' << E << ',' << match << ',' << duration << std::endl;
        };

        run(HopcroftKarp0(L, R), "HopcroftKarp0");
        run(HopcroftKarp1(L, R), "HopcroftKarp1");

        std::cerr << "Test " << entry.path().filename() << " complete" << std::endl;
        Close();
    }

    output.close();
    return 0;
}