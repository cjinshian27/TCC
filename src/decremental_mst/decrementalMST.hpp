#include "./helpful_classes/node.hpp"
#include "./helpful_classes/tree.hpp"
#include "./helpful_classes/forest.hpp"
#include "adjacencyList.hpp"

void printStylishLine(){
	std::cout << "════════════════════════════════════════════════════════════════════════════════════\n";
}

template<typename Key>

class DecrementalMST{
	
	private:

		// store the adjacency lists (each one stores backup nodes)
		std::vector<AdjacencyList<Key> *> adjacencyLists;
		
		// store the forests 
		std::vector<Forest<Key> *> forests;
		
		// map the this->maxLevel of the nodes 
		std::unordered_map<Key, std::unordered_map<Key, unsigned int>> mapNodeLevels;
		
		unsigned int maxLevel;
		
		void updateMapNodeLevels(Key u, Key v, unsigned int level){
			this->mapNodeLevels[u][v] = level;
			this->mapNodeLevels[v][u] = level;
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
		void decreaseNodesLevel(Tree<Key> * uTree, unsigned int i){
			
			Node<Key> * nodeToSplay = uTree->getNodeWithIsLevelTrue(uTree->root);
			
			uTree->splay(nodeToSplay);
			uTree->root->isLevel = 0;
			uTree->root->setNodeLevelCount();
			
			Key u = uTree->root->first;
			Key v = uTree->root->second;
			updateMapNodeLevels(u, v, i - 1);
			this->forests[i - 1]->link(u, v);
		}	
		
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
		void replaceNode(Key u, Key v, unsigned int nodeLevel){
			
			bool nodeIsReplaced = false;
				
			for(unsigned int i = nodeLevel; i <= this->maxLevel && !nodeIsReplaced; ++i){
				
				Tree<Key> * uTree = this->forests[i]->getTreeContaining(u);
				Tree<Key> * treeV = this->forests[i]->getTreeContaining(v);
				
				if(uTree->size() > treeV->size()){
					std::swap(uTree, treeV);
					std::swap(u, v);
				} 
				
				while(uTree->root->nodesAtLevel > 0){
					decreaseNodesLevel(uTree, i);
				}
					
					
				while(uTree->root->reserveNodes > 0 && !nodeIsReplaced){
					
					Node<Key> * xxVertex = uTree->getReserveNode(uTree->root);
					uTree->splay(xxVertex);
					std::vector<Key> reserveNodesToBeRemoved;
					Key x = xxVertex->first;

					for (const Key & y : this->adjacencyLists[i]->adjList[x]) {
						
						reserveNodesToBeRemoved.push_back(y);	
						
						//y does not belong to Tv 
						if(this->forests[i]->isConnected(x, y)){
							updateMapNodeLevels(x, y, i - 1);
							this->adjacencyLists[i - 1]->add(x, y);
						}
						else{
							for(unsigned int j = i; j <= this->maxLevel; ++j){
								this->forests[j]->link(x, y);
							}
							nodeIsReplaced = true;
							break;
						}
					}
					
					//remove y nodes that are incident to x but not in vTree
					for (Key & y : reserveNodesToBeRemoved) {
						this->adjacencyLists[i]->remove(x, y);
						this->forests[i]->decreaseIncidentToReserveNodeCount(y);
					}	

					// if x is not incident to any other node of level i, then
					// decrease the reserve node count
					if(this->adjacencyLists[i]->adjList[x].empty()){
						this->forests[i]->decreaseIncidentToReserveNodeCount(x);
						xxVertex->isIncidentToReserveNode = false;
						xxVertex->setReserveNodesCount();
					}
				}
			}
		}
		
		// sort m edges by increasing weight, which takes O(mlog(m)) time
		void sortEdges(std::vector<std::tuple> & edges){
			std::sort(edges.begin(), edges.end(), 
				[](const auto& a, const auto& b) {
					return std::get<2>(a) < std::get<2>(b);
				});
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
		// add node (u, v) in O(lg(n))
		void add(Key u, Key v, int weight){
	
			updateMapNodeLevels(u, v, this->maxLevel);
			Forest<Key> * maxLevelForest = this->forests[this->maxLevel];
	
			if(maxLevelForest->isConnected(u, v)){
				this->adjacencyLists[this->maxLevel]->add(u, v);
				maxLevelForest->increaseIncidentToReserveNodeCount(u);
				maxLevelForest->increaseIncidentToReserveNodeCount(v);
			} 
			else{
				maxLevelForest->link(u, v);
			} 
		}

	public:

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		DecrementalMST(unsigned int numberOfVertices, std::vector<std::tuple<Key, Key, int>> & edges){
			
			this->maxLevel = static_cast<int>(std::ceil(std::log2(numberOfVertices))); 
			this->adjacencyLists = std::vector<AdjacencyList<Key> *>(this->maxLevel + 1);
			this->forests = std::vector<Forest<Key> *>(this->maxLevel + 1);
			
			for(unsigned int i = 0; i <= this->maxLevel; ++i){
				adjacencyLists[i] = new AdjacencyList<Key>();
				forests[i] = new Forest<Key>(numberOfVertices);
			}

			sortEdges(edges);
			
			for(unsigned int i = 0; i < edges.size(); ++i){
				add(
					std::get<0>(edges[i]),
					std::get<1>(edges[i]),
					std::get<2>(edges[i])
				);
			}
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		void remove(Key u, Key v){
			
			unsigned int nodeLevel = mapNodeLevels[u][v];
			mapNodeLevels[u].erase(v);
			mapNodeLevels[v].erase(u);

			if(this->forests[this->maxLevel]->hasNode(u, v)){
				for(unsigned int i = nodeLevel; i <= this->maxLevel; ++i)
					 this->forests[i]->cut(u, v);
	
				replaceNode(u, v, nodeLevel);
			}
			else{
				this->adjacencyLists[this->maxLevel]->remove(u, v);
				this->forests[this->maxLevel]->decreaseIncidentToReserveNodeCount(u);
				this->forests[this->maxLevel]->decreaseIncidentToReserveNodeCount(v);
			}
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// check if u and v are connected in O(lg(n))
		bool isConnected(Key u, Key v){
			
			Forest<Key> * forest = this->forests[this->maxLevel];
			return forest->isConnected(u, v);
		}

		// prints out all the dynamic graph in O(nlg(n))
		void print(){
			printStylishLine();
			std::cout << "FORESTS: \n\n";
			for(int i = this->maxLevel; i >= 0; --i){
				std::cout << "FOREST LEVEL " << i + 1 << ":\n";
				this->forests[i]->printTrees();
				std::cout << "**************************************************\n";
			}

			std::cout << "\n\n\n\n"; 

			std::cout << "ADJACENCY LISTS \n\n";
			for(int i = this->maxLevel; i >= 0; --i){
				std::cout << "ADJACENCY LIST LEVEL " << i + 1 << ":\n";
				this->adjacencyLists[i]->print();
				std::cout << "**************************************************\n";
			}
			printStylishLine();
		}
};