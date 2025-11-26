    #include <vector>
    #include <tuple>
    #include <random>
    #include <iostream>
    #include <cmath>
    #include <chrono>

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

    // code to generate a weighted G(n, p) graph
    int main() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr);

        const long long n = 20000;

        const double p = log2(20000.0) / 20000.0;  // ≈ 0.000714385

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

        std::vector<std::tuple<int, int, int>> edgesToRemove = buildPermutation(edges, rng, m);

        std::cout << "# G(" << n << ", " << p << ")\n";
        std::cout << "# seed = " << seed << "\n";
        std::cout << "# vertices = " << n << "\n";
        std::cout << "# edges (actual) = " << m << "\n";
        std::cout << "# edges (expected) = " << expectedNumberOfEdges << "\n";
        std::cout << "# weights: [" << minWeight << ", " << maxWeight << "]\n";
        std::cout << "# format: u v w\n";

        return 0;
    }
