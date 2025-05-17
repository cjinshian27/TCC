#include <iostream>
#include <cmath>
#include "./helpful_classes/forest.hpp"
#include "./helpful_classes/tree.hpp"
#include "./helpful_classes/edge.hpp"
#include "adjacencyList.hpp"

template<typename Key>

class DynamicGraph{

	private:

		// store the adjacency lists (each one stores backup edges)
		std::vector<AdjacencyList<Key> *> * adjacencyLists;
		
		// store the forests 
		std::vector<Forest<Key> *> * forests;

		// map the this->maxLevel of the edges 
		std::unordered_map<Key, std::unordered_map<Key, unsigned int>> mapEdgeLevels;
		
		unsigned int maxLevel;

		void replaceEdge(Key u, Key v, unsigned int edgeLevel){
			
			for(unsigned int i = edgeLevel; i <= this->maxLevel; ++i){
				
				Tree<Key> * treeU = this->forests[i]->getTreeContaining(u);
				Tree<Key> * treeV = this->forests[i]->getTreeContaining(v);

				if(treeU->size() > treeV->size()){
					std::swap(treeU, treeV);
					std::swap(u, v);
				} 
				
				while(treeU->root->countVerticesAtLevel(i) > 0){
					treeU->decreaseEdgesLevel(treeU->root);
					
					Key u = treeU->root.first;
					Key v = treeU->root.second;
					mapEdgeLevels[u][v] = i - 1;
					this->forests[i - 1]->link(u, v);
				}
				


				while(){

					this->adjacencyLists[i]->remove(x, y);
					
					if(treeV->hasEdge(y)){
						for(unsigned int j = edgeLevel; j <= this->maxLevel; ++j){
							this->forests[j]->link(x, y);
						}
						return;
					}
					else{
						mapEdgeLevels[x][y] = i - 1;
						this->adjacencyLists[i - 1]->add(x, y);
					}
				}
			}
		}

	public:

		// instantiate a dynamic graph in O(lg(n)) time.
		DynamicGraph(vector<Key> & vertices){
			
			this->maxLevel = static_cast<int>(std::ceil(std::log2(n))); 

			this->adjacencyLists = new std::vector<AdjacencyList<Key> *>(this->maxLevel + 1);
			this->forests = new std::vector<Forest<Key> *>(this->maxLevel + 1);
			
			for(int i = 0; i <= this->maxLevel; ++i){
				adjacencyLists[i] = new AdjacencyList<Key>();
				forests[i] = new Forest<Key>(vertices);
			}
		}

		// add edge (u, v) in O(lg(n))
		void add(Key u, Key v){

			mapEdgeLevels[u][v] = this->maxLevel;

			AdjacencyList<Key> * adjList = this->adjacencyLists[this->maxLevel];
			Forest<Key> * forest = this->forests[this->maxLevel];

			if(forest->isConnected(u, v)) adjList->add(u, v);
			else forest->add(u, v);
		}

		void remove(Key u, Key v){
			
			unsigned int edgeLevel = mapEdgeLevels[u][v];
			mapEdgeLevels[u].erase(v);

			if(this->forests[this->maxLevel]->hasEdge(u, v)){
				for(int i = edgeLevel; i <= this->maxLevel; ++i)
					 this->forests[i]->cut(u, v);
	
				replaceEdge(u, v, edgeLevel);
			}
			else{
				this->adjacencyLists[this->maxLevel]->remove(u, v);
			}
		}

		// check if u and v are connected in O(lg(n))
		bool isConnected(Key u, Key v){
			
			Forest<Key> * forest = this->forests[this->maxLevel];
			return forest->isConnected(u, v);
		}
}