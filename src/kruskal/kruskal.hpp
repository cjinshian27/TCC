#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "edge.hpp"
#include "union_find.hpp"

#include "../decremental_minimum_spanning_forest/helpful_classes/robin_hood.h"
#include "../decremental_minimum_spanning_forest/helpful_classes/edgeHash.hpp"

class Graph {
    private:
        int V;
        std::vector<GraphEdge> edges;
        std::vector<GraphEdge> mst_edges;
        UnionFind * uf;
        long long mst_weight;

        robin_hood::unordered_map<EdgeKey, bool, EdgeKeyHash> removedEdges;
        
        robin_hood::unordered_map<EdgeKey, bool, EdgeKeyHash> treeEdges;

    public:
        Graph(int vertices) : V(vertices), mst_weight(0) {
            uf = new UnionFind(V);
        }
        
        ~Graph() {
            delete uf;
        }
        
        // and an weighted edge
        void addEdge(int u, int v, int weight) {
            
            if(u > v) std::swap(u, v);
            edges.emplace_back(u, v, weight);
        }
        
        // remove an edge
        void removeEdge(int u, int v) {
            
            if(u > v) std::swap(u, v);
            removedEdges[{u, v}] = true;
            
            if(treeEdges[{u, v}]){
                buildMST();
            }
        }
        
        void sortEdges(){
            sort(edges.begin(), edges.end());

        }
        // build MST using Kruskal's algorithm
        void buildMST() {
            
            delete uf;
            uf = new UnionFind(V);
            mst_edges.clear();
            mst_weight = 0;
                                
            // process edges in order
            for (const auto& edge : edges) {
                
                if (!removedEdges[{edge.u, edge.v}] && uf->unite(edge.u, edge.v)) {
                    
                    treeEdges[{edge.u, edge.v}] = true;
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
        void printTotalWeight(){
            std::cout << "total weight is: " << mst_weight << '\n';
        }

        // print MST weighted edges
        void printWeightedEdges() {
            for (const auto& edge : mst_edges) {
                std::cout << "(" << edge.u << ", " << edge.v << ", weight: " << edge.weight  << ")\n";
            }
        }

};
