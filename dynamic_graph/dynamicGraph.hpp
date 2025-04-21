#include <iostream>
#include <cmath>
#include "./helpful_classes/dynamicForest.hpp"
#include "adjacencyList.hpp"

template<typename Key>

class DynamicGraph{

	private:

		// store the adjacency lists (each adjacency list store backup edges)
		std::vector<AdjacencyList<Key> *> * adjacencyLists;
		
		// store the dynamic forests 
		std::vector<Forest<Key> *> * forests;

		// map the level of the edges 
		std::unordered_map<Key, std::unordered_map<Key, unsigned int>> mapEdgeLevels;
		
		unsigned int level;
		
		void replaceEdge(Key u, Key v, unsigned int edgeLevel){
			
			for(unsigned int i = edgeLevel; i <= level; ++i){
			}
			
		}


	public:

		// instantiate a dynamic graph in O(lg(n)) time.
		DynamicGraph(unsigned int n){
			
			this->level = static_cast<int>(std::ceil(std::log2(n))); 

			this->adjacencyLists = new std::vector<AdjacencyList<Key> *>(level + 1);
			this->forests = new std::vector<Forest<Key> *>(level + 1);
			
			for(int i = 0; i <= level; ++i){
				adjacencyLists[i] = new AdjacencyList<Key>();
				forests[i] = new Forest<Key>();
			}
		}

		// add edge (u, v) in O(lg(n))
		void add(Key u, Key v){

			mapEdgeLevels[u][v] = level;

			AdjacencyList<Key> * adjList = this->adjacencyLists[level];
			Forest<Key> * forest = this->forests[level];

			if(forest->isConnected(u, v)) adjList->insert(u, v);
			else forest->add(u, v);
		}

		void remove(Key u, Key v){
			
			unsigned int edgeLevel = mapEdgeLevels[u][v];
			mapEdgeLevels[u].erase(v);

			if(this->forests[level]->hasEdge(u, v)){
				for(int i = edgeLevel; i <= level; ++i){
					this->forests[i]->cut(u, v);
				}

				replaceEdge(u, v, edgeLevel);
			}
			else{
				this->adjacencyLists[level]->removeEdge(u, v);
			}
		}

		// check if u and v are connected in O(lg(n))
		bool isConnected(Key u, Key v){
			
			Forest<Key> * forest = this->forests[level];
			return forest->isConnected(u, v);
		}
}