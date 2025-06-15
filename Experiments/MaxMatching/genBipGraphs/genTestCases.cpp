#include <iostream>
#include <string>

#include "headers/genCycles.hpp"
#include "headers/genKhunKiller.hpp"
#include "headers/genLarge.hpp"
#include "headers/genLayer.hpp"
#include "headers/genLine.hpp"
#include "headers/genLineRandom.hpp"
#include "headers/genManyPaths.hpp"
#include "headers/genManySmall.hpp"
#include "headers/genRnd.hpp"
#include "headers/genMaxRnd.hpp"

void genCyles() {
    int idx = 0;
    int L = 10'000, R = 10'000;
    for (int i = 0; i < 2; ++i) {
        std::string filename = "../testCases/cycles-" + std::to_string(idx) + ".in";
        genCycle(L, R, i, filename);
        ++idx;
    }

    L = 50'000, R = 50'000;
    for (int i = 0; i < 2; ++i) {
        std::string filename = "../testCases/cycles-" + std::to_string(idx) + ".in";
        genCycle(L, R, i, filename);
        ++idx;
    }

    L = 100'000, R = 100'000;
    for (int i = 0; i < 2; ++i) {
        std::string filename = "../testCases/cycles-" + std::to_string(idx) + ".in";
        genCycle(L, R, i, filename);
        ++idx;
    }

    L = 200'000, R = 200'000;
    for (int i = 0; i < 2; ++i) {
        std::string filename = "../testCases/cycles-" + std::to_string(idx) + ".in";
        genCycle(L, R, i, filename);
        ++idx;
    }
}

void genKhunKiller() {
    int idx = 0;
    int L = 100'000, R = 100'000;
    for (int i = 0; i < 5; ++i) {
        std::string filename = "../testCases/khun-killer-" + std::to_string(idx) + ".in";
        genKhunKiller(L, R, i, filename);
        ++idx;
    }

    L = 200'000, R = 200'000;
    for (int i = 0; i < 5; ++i) {
        std::string filename = "../testCases/khun-killer-" + std::to_string(idx) + ".in";
        genKhunKiller(L, R, i, filename);
        ++idx;
    }
}

void genLarge() {
    int idx = 0;
    int L = 100'000, R = 100'000, M = 100'000;
    for (int i = 0; i < 5; ++i) {
        std::string filename = "../testCases/large-" + std::to_string(idx) + ".in";
        genLarge(L, R, M, i, filename);
        M += 100'000; 
        ++idx;
    }

    L = 200'000, R = 200'000, M  = 100'000;
    for (int i = 0; i < 5; ++i) {
        std::string filename = "../testCases/large-" + std::to_string(idx) + ".in";
        genLarge(L, R, M, i, filename);
        ++idx;
        M += 100'000;
    }
}

void genLayer() {
    int idx = 0;
    int B = 12;
    for (int i = 0; i < 5; ++i) {
        std::string filename = "../testCases/layer-" + std::to_string(idx) + ".in";
        genLayer(B, i, filename);
        ++idx;
    }
}

void genLine() {
    int idx = 0;
    int L = 10'000, R = 10'000;
    for (int i = 0; i < 2; ++i) {
        std::string filename = "../testCases/line-" + std::to_string(idx) + ".in";
        genLine(L, R, i, filename);
        ++idx;
    }

    L = 50'000, R = 50'000;
    for (int i = 0; i < 2; ++i) {
        std::string filename = "../testCases/line-" + std::to_string(idx) + ".in";
        genLine(L, R, i, filename);
        ++idx;
    }

    L = 100'000, R = 100'000;
    for (int i = 0; i < 2; ++i) {
        std::string filename = "../testCases/line-" + std::to_string(idx) + ".in";
        genLine(L, R, i, filename);
        ++idx;
    }

    L = 200'000, R = 200'000;
    for (int i = 0; i < 2; ++i) {
        std::string filename = "../testCases/line-" + std::to_string(idx) + ".in";
        genLine(L, R, i, filename);
        ++idx;
    }
}

void genLineRandom() {
    int idx = 0;
    int L = 10'000, R = 10'000;
    for (int i = 0; i < 2; ++i) {
        std::string filename = "../testCases/linernd-" + std::to_string(idx) + ".in";
        genLine(L, R, i, filename);
        ++idx;
    }

    L = 50'000, R = 50'000;
    for (int i = 0; i < 2; ++i) {
        std::string filename = "../testCases/linernd-" + std::to_string(idx) + ".in";
        genLineRnd(L, R, i, filename);
        ++idx;
    }

    L = 100'000, R = 100'000;
    for (int i = 0; i < 2; ++i) {
        std::string filename = "../testCases/linernd-" + std::to_string(idx) + ".in";
        genLineRnd(L, R, i, filename);
        ++idx;
    }

    L = 200'000, R = 200'000;
    for (int i = 0; i < 2; ++i) {
        std::string filename = "../testCases/linernd-" + std::to_string(idx) + ".in";
        genLineRnd(L, R, i, filename);
        ++idx;
    }
}

void genManyPaths() {
    int idx = 0;
    int L = 200'000, R = 200'000, M = 200'000;
    for (int i = 0; i < 5; ++i) {
        std::string filename = "../testCases/manypaths-" + std::to_string(idx) + ".in";
        genManyPaths(L, R, M, i, filename);
        M += 100'000; 
        ++idx;
    }

    L = 100'000, R = 100'000, M  = 200'000;
    for (int i = 0; i < 5; ++i) {
        std::string filename = "../testCases/manypaths-" + std::to_string(idx) + ".in";
        genManyPaths(L, R, M, i, filename);
        ++idx;
        M += 100'000;
    }
}

void genManySmall() {
    int idx = 0;
    int L = 200'000, R = 200'000, M = 400'000;
    for (int i = 0; i < 10; ++i) {
        std::string filename = "../testCases/manysmall-" + std::to_string(idx) + ".in";
        genManySmall(L, R, M, i, filename);
        ++idx;
    }
}

void genRnd() {
    int idx = 0;
    int L = 200'000, R = 200'000, M  = 500'000;
    for (int i = 0; i < 10; ++i) {
        std::string filename = "../testCases/rnd-" + std::to_string(idx) + ".in";
        genRnd(L, R, M, i, filename);
        ++idx;
    }
}

void genMaxRnd() {
    int idx = 0;
    int L = 200'000, R = 200'000, M  = 500'000;
    for (int i = 0; i < 10; ++i) {
        std::string filename = "../testCases/mxrnd-" + std::to_string(idx) + ".in";
        genMaxRnd(L, R, M, i, filename);
        ++idx;
    }
}

int main() {
    genCyles();
    genKhunKiller();
    genLarge();
    genLayer();
    genLine();
    genLineRandom();
    genManyPaths();
    genManySmall();
    genRnd();
    genMaxRnd();
    return 0;
}