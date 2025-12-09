#include <vector>
#include <tuple>
#include <random>
#include <iostream>
#include <cmath>
#include <chrono>
#include <fstream>

std::vector<std::tuple<int, int, int>> buildPermutation(std::vector<std::tuple<int,int,int>> & edges, std::mt19937_64 & rng, int m){

    std::vector<int> perm(m);
    for (int i = 0; i < m; i++) perm[i] = i;

    for (int i = m - 1; i > 0; i--) {
        std::uniform_int_distribution<int> dist(0, i);
        int r = dist(rng);
        std::swap(perm[i], perm[r]);
    }

    std::vector<std::tuple<int,int,int>> shuffledEdges;
    shuffledEdges.resize(m);

    for (int i = 0; i < m; i++) {
        shuffledEdges[i] = edges[perm[i]];
    }

    return shuffledEdges;
}

void buildFile(std::vector<std::tuple<int, int, int>> & edges, std::vector<std::tuple<int, int, int>> & edgesToRemove, int n, int m, int graphID, int permutationID){
    
    std::string filename =
        "graph_" + std::to_string(graphID) +
        "_permutation_" + std::to_string(permutationID);

    std::ofstream out(filename);

    if (!out) {
        std::cerr << "Error opening output file.\n";
        return;
    }

    // first line: n m
    out << n << " " << m << "\n";

    // next m lines: generated edges u v w
    for (auto &e : edges) {
        int u, v, w;
        std::tie(u, v, w) = e;
        out << u << " " << v << " " << w << "\n";
    }

    // next m lines: removal operations "2 u v"
    for (auto &e : edgesToRemove) {
        int u, v, w;
        std::tie(u, v, w) = e;
        out << "2 " << u << " " << v << "\n";
    }

    // last line: 3 (MST weight)
    out << "3\n";

    out.close();

    std::cout << "File graph_"<< graphID << "_permutation_" << permutationID << " written successfully.\n";
}


void generateGraph(int graphID, int numberOfPermutations){
    const long long n = 20000;

    const double p = log2(n) / n;  // ≈ 0.000714385

    const long long minWeight = 1;
    const long long maxWeight = n;

    // random seed from system
    std::random_device rd;
    unsigned long long seed =
        (unsigned long long)rd() ^
        ((unsigned long long) std::chrono::high_resolution_clock::now().time_since_epoch().count() );

    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> real01(0.0, 1.0);
    std::uniform_int_distribution<long long> weightDist(minWeight, maxWeight);

    std::vector<std::tuple<int,int,int>> edges;
    edges.reserve((long long)((n*(n-1)/2.0) * p)); // rough expected ~ 142k edges

    // generate edges
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (real01(rng) < p) {
                int w = weightDist(rng);
                edges.emplace_back(i, j, w);
            }
        }
    }

    long long m = edges.size();
    double expectedNumberOfEdges = (n * (n - 1) / 2.0) * p;

    // std::cout << "# G(" << n << ", " << p << ")\n";
    // std::cout << "# seed = " << seed << "\n";
    // std::cout << "# vertices = " << n << "\n";
    // std::cout << "# edges (actual) = " << m << "\n";
    // std::cout << "# edges (expected) = " << expectedNumberOfEdges << "\n";
    // std::cout << "# weights: [" << minWeight << ", " << maxWeight << "]\n";
    // std::cout << "# format: u v w\n";
    for(unsigned int permutationID = 0; permutationID < numberOfPermutations; ++permutationID){

        std::vector<std::tuple<int, int, int>> edgesToRemove = buildPermutation(edges, rng, m);
    
        buildFile(edges, edgesToRemove, n, m, graphID,  permutationID);
    }
}

// code to generate a weighted G(n, p) graph
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int numberOfGraphs = 6;
    int numberOfPermutations = 5;
    
    for(unsigned int i = 0; i < numberOfGraphs; ++i){
        generateGraph(i, numberOfPermutations);
    }

    return 0;
}
