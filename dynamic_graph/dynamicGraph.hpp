#include "./helpful_classes/edge.hpp"
#include "./helpful_classes/tree.hpp"
#include "./helpful_classes/forest.hpp"
#include "adjacencyList.hpp"

template<typename Key>

class DynamicGraph{

	private:

		// store the adjacency lists (each one stores backup edges)
		std::vector<AdjacencyList<Key> *> adjacencyLists;
		
		// store the forests 
		std::vector<Forest<Key> *> forests;

		// map the this->maxLevel of the edges 
		std::unordered_map<Key, std::unordered_map<Key, unsigned int>> mapEdgeLevels;
		
		unsigned int maxLevel;
		
		void updateMapEdgeLevels(Key u, Key v, unsigned int level){
			this->mapEdgeLevels[u][v] = level;
			this->mapEdgeLevels[v][u] = level;
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		void decreaseEdgesLevel(Tree<Key> * uTree, unsigned int i){
			
			Edge<Key> * edgeToSplay = uTree->getEdgeWithIsLevelTrue(uTree->root);
			
			uTree->splay(edgeToSplay);
			uTree->root->isLevel = 0;
			uTree->root->setEdgeLevelCount();

			Key u = uTree->root->edge.first;
			Key v = uTree->root->edge.second;
			updateMapEdgeLevels(u, v, i - 1);
			this->forests[i - 1]->link(u, v);
		}	

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		void replaceEdge(Key u, Key v, unsigned int edgeLevel){
			
			for(unsigned int i = edgeLevel; i <= this->maxLevel; ++i){
				
				Tree<Key> * uTree = this->forests[i]->getTreeContaining(u);
				Tree<Key> * treeV = this->forests[i]->getTreeContaining(v);

				if(uTree->size() > treeV->size()){
					std::swap(uTree, treeV);
					std::swap(u, v);
				} 
				
				while(uTree->root->edgesAtLevel > 0){
					decreaseEdgesLevel(uTree, i);
				}

				bool edgeIsReplaced = false;
				
				while(uTree->root->reserveEdges > 0 && !edgeIsReplaced){
					
					Edge<Key> * xVertex = uTree->getReserveEdge(uTree->root);
					xVertex->isIncidentToReserveEdge = false;
					xVertex->setReserveEdgesCount();
					std::vector<Key> reserveEdgesToBeRemoved;
					Key x = xVertex->edge.first;

					for (Key & y : this->adjacencyLists[i][x]){
						
						reserveEdgesToBeRemoved.push_back(y);	
						
						//y does not belong to Tv 
						if(this->forests[i]->isConnected(x, y)){
							updateMapEdgeLevels(x, y, i - 1);
							this->adjacencyLists[i - 1]->add(x, y);
						}
						else{
							for(unsigned int j = i; j <= this->maxLevel; ++j){
								this->forests[j]->link(x, y);
							}
							edgeIsReplaced = true;
							break;
						}
					}
					
					for(Key & y : reserveEdgesToBeRemoved){
						this->adjacencyLists[i]->remove(x, y);
						this->forests[i]->decreaseEdgeLevelCount(y);
					}
				}
			}
		}

	public:

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// instantiate a dynamic graph in O(lg(n)) time.
		DynamicGraph(std::vector<Key> & vertices){
			
			this->maxLevel = static_cast<int>(std::ceil(std::log2(vertices.size()))); 

			this->adjacencyLists = std::vector<AdjacencyList<Key> *>(this->maxLevel + 1);
			this->forests = std::vector<Forest<Key> *>(this->maxLevel + 1);
			
			for(int i = 0; i <= this->maxLevel; ++i){
				adjacencyLists[i] = new AdjacencyList<Key>();
				forests[i] = new Forest<Key>(vertices);
			}
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// add edge (u, v) in O(lg(n))
		void add(Key u, Key v){

			updateMapEdgeLevels(u, v, this->maxLevel);

			if(this->forests[this->maxLevel]->isConnected(u, v)){
				this->adjacencyLists[this->maxLevel]->add(u, v);
			} 
			else{
				this->forests[this->maxLevel]->link(u, v);
			} 
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		void remove(Key u, Key v){
			
			unsigned int edgeLevel = mapEdgeLevels[u][v];
			mapEdgeLevels[u].erase(v);
			mapEdgeLevels[v].erase(u);

			if(this->forests[this->maxLevel]->hasEdge(u, v)){
				for(int i = edgeLevel; i <= this->maxLevel; ++i)
					 this->forests[i]->cut(u, v);
	
				replaceEdge(u, v, edgeLevel);
			}
			else{
				this->adjacencyLists[this->maxLevel]->remove(u, v);
			}
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// check if u and v are connected in O(lg(n))
		bool isConnected(Key u, Key v){
			
			Forest<Key> * forest = this->forests[this->maxLevel];
			return forest->isConnected(u, v);
		}
};