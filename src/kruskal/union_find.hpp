#pragma once

#include <vector>

class UnionFind {
    private:
        std::vector<int> parent;
        std::vector<int> rank;
        int num_components;

    public:
        UnionFind(int n) : parent(n), rank(n, 0), num_components(n) {
            for (int i = 0; i < n; i++) {
                parent[i] = i;
            }
        }
        
        // Find with path compression
        int find(int x) {
            if (parent[x] != x) {
                parent[x] = find(parent[x]);  // Path compression
            }
            return parent[x];
        }
        
        // Union by rank
        bool unite(int x, int y) {
            int rootX = find(x);
            int rootY = find(y);
            
            if (rootX == rootY) return false;  // Already connected
            
            // Union by rank
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
            
            num_components--;
            return true;
        }
        
        // Check if two nodes are connected
        bool connected(int x, int y) {
            return find(x) == find(y);
        }
        
        int getNumComponents() const {
            return num_components;
        }
};
