#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "edge.hpp"
#include "union_find.hpp"

// Graph class with Kruskal's algorithm
class Graph {
private:
    int V;  // number of vertices
    std::vector<Edge> edges;
    std::vector<Edge> mst_edges;
    UnionFind* uf;
    long long mst_weight;
    
    // make u < v
    std::pair<int,int> normalize_edge(int u, int v) {
        if (u > v) std::swap(u, v);
        return {u, v};
    }

    void insertionSort(std::vector<Edge>& edges) {
        int n = edges.size();
        for (int i = 1; i < n; i++) {
            Edge key = edges[i];
            int j = i - 1;

            while (j >= 0 && edges[j].weight > key.weight) {
                edges[j + 1] = edges[j];
                j--;
            }

            edges[j + 1] = key;
        }
    }

        // build MST using Kruskal's algorithm
    void rebuildMST() {
        
        // reset Union-Find and MST data
        delete uf;
        uf = new UnionFind(V);
        mst_edges.clear();
        mst_weight = 0;
        
        // sort in O(n) time in a nearly sorted array
        insertionSort(edges);
                
        // process edges in order
        for (const auto& edge : edges) {
            if (uf->unite(edge.u, edge.v)) {
                mst_edges.push_back(edge);
                mst_weight += edge.weight;
                
                // MST complete when we have V-1 edges
                if (mst_edges.size() == V - 1) break;
            }
        }
        
    }


public:
    Graph(int vertices) : V(vertices), mst_weight(0){
        uf = new UnionFind(V);
    }
    
    ~Graph() {
        delete uf;
    }
    
    // and an weighted edge
    void addEdge(int u, int v, int weight) {
        edges.emplace_back(u, v, weight);
    }
    
    // remove an edge
    void removeEdge(int u, int v) {
        std::pair<int,int> target_edge = normalize_edge(u, v);
        
        // Find the edge in the vector
        bool found = false;
        for (int i = 0; i < edges.size(); i++) {
            if (normalize_edge(edges[i].u, edges[i].v) == target_edge) {
                std::swap(edges[i], edges[edges.size() - 1]);
                
                edges.pop_back();
                
                found = true;

                break;
            }
        }
        
        if (!found) {
            return;
        }
        rebuildMST();
    }

    // build MST using Kruskal's algorithm
    void buildMST() {
        
        // sort edges by increasing weight
        sort(edges.begin(), edges.end());
                
        // process edges in order
        for (const auto& edge : edges) {
            if (uf->unite(edge.u, edge.v)) {
                mst_edges.push_back(edge);
                mst_weight += edge.weight;
                
                // MST complete when we have V-1 edges
                if (mst_edges.size() == V - 1) break;
            }
        }
    }
    
    // check connectivity between two vertices
    bool areConnected(int u, int v) {        
        if (u < 0 || u >= V || v < 0 || v >= V) {
            return false;
        }
        return uf->connected(u, v);
    }
    
    // get number of components
    int getNumComponents() {
        return uf->getNumComponents();
    }
    
    // print MST total weight
    long long getTotalWeight(){
        return mst_weight;
    }

    // print MST weighted edges
    void printWeightedEdges() {
        for (const auto& edge : mst_edges) {
            std::cout << "(" << edge.u << ", " << edge.v << ", weight: " << edge.weight  << ")\n";
        }
    }

};
