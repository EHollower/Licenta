#pragma once

#include <vector>
#include <random>
#include <utility>
#include <iostream>
#include <algorithm>

struct Edge {
    int from, to, cap;
};

void genLayeredPath(int segmentCount, const long long &seed, const std::string &filename) {
    (void)!freopen(filename.c_str(), "w", stdout);

    const int INITIAL_NODES  = 2;
    const int CONSTANT_SN    = 500;
    const int PATH_STEP      = 50;
    const int SEGMENT_LEN    = 25;
    const int OFFSET         = 847;

    int nodeCount = INITIAL_NODES;
    std::vector <Edge> edges;

    edges.push_back({0, 1, 0});
    int lastSnuke = -1;

    for (int seg = 0; seg < segmentCount; ++seg) {
        int startNode  = 0;
        int baseOffset = OFFSET + seg * PATH_STEP;

        if (lastSnuke >= 0) edges.push_back({lastSnuke, nodeCount + CONSTANT_SN, SEGMENT_LEN});
        lastSnuke = nodeCount + CONSTANT_SN;

        for (int i = 0; i < baseOffset; ++i) {
            edges.push_back({startNode, nodeCount, SEGMENT_LEN});
            startNode = nodeCount++;
        }

        for (int i = 0; i < SEGMENT_LEN; ++i) {
            edges.push_back({startNode, nodeCount, SEGMENT_LEN});
            startNode = nodeCount++;
            int cap = (i < SEGMENT_LEN - 1) ? 1 : SEGMENT_LEN;
            edges.push_back({startNode, 1, cap});
        }

        int endFirst = startNode;

        startNode = 0;
        edges.push_back({lastSnuke, nodeCount + CONSTANT_SN, SEGMENT_LEN});
        lastSnuke = nodeCount + CONSTANT_SN;

        for (int i = 0; i < baseOffset + SEGMENT_LEN + 1; ++i) {
            edges.push_back({startNode, nodeCount, SEGMENT_LEN});
            startNode = nodeCount++;
        }
        
        int endSecond = startNode;
        for (int back = 0; back < SEGMENT_LEN - 1; ++back) {
            edges.push_back({endSecond, endFirst - back, 1});
        }
    }

    int edgeCount = edges.size();
    std::cout << nodeCount << " " << edgeCount << " " << 0 << " " << nodeCount - 1 << "\n";

    std::vector <int> labelMap(nodeCount);
    std::iota(labelMap.begin(), labelMap.end(), 0);
    std::mt19937 rng(seed);
    std::shuffle(labelMap.begin() + 1, labelMap.end(), rng);

    for (auto &e : edges) {
        e.from = labelMap[e.from];
        e.to   = labelMap[e.to];
    }

    shuffle(edges.begin(), edges.end(), rng);

    for (const auto &e : edges) std::cout << e.from  << " " << e.to << " " << e.cap << "\n";
}


/*
# References:
https://ideone.com/LKx3FP
https://codeforces.com/blog/entry/80627
*/
