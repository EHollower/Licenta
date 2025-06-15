#include "headers/random.hpp"
#include "headers/complete.hpp"
#include "headers/layered_path.hpp"
#include "headers/bottlenecks.hpp"

#include <iostream>

void genTestCasesRnd() {
    for (int i = 100; i <= 1000; i += 100) {
        for (int j = 0; j < 5; ++j) {
            std::string filename = "../testCases/random-" + std::to_string(i) + "-" + std::to_string(j) + ".in";
            genRandomFlowGraph(i - 2, j, filename);
        }
    }
}

void genTestCasesLayered() {
    for (int i = 1; i <= 5; ++i) {
        std::string filename = "../testCases/layered-" + std::to_string(i) + ".in";
        for (int j = 0; j < 5; ++j) {
            filename = "../testCases/layered-" + std::to_string(i) + "-" + std::to_string(j) + ".in";
            genLayeredPath(i, j, filename);
        }
    }
}

void genTestCasesComplete() {
    for (int i = 1000; i <= 3000; i += 1000) {
        for (int j = 0; j < 5; ++j) {
            std::string filename = "../testCases/complete-" + std::to_string(i) + "-" + std::to_string(j) + ".in";
            genCompleteFlowGraph(i - 2, j, filename);
        }
    }
}

void genTestCaseBottlenecks() {
    for (int i = 200; i <= 2000; i += 200) {
        for (int j = 0; j < 2; ++j) {
            std::string filename = "../testCases/bottleneck-" + std::to_string(i) + "-" + std::to_string(j) + ".in";
            genTestCaseBottle(i, j, filename);
        }
    }
}

int main() {
    genTestCasesRnd();
    genTestCasesLayered();
    genTestCasesComplete();
    genTestCaseBottlenecks();
    return 0;
}
