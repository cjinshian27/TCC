#include "./helpful_classes/node.hpp"
#include "./helpful_classes/tree.hpp"
#include "./helpful_classes/forest.hpp"
#include "adjacencyList.hpp"


void printStylishLine(){
	std::cout << "════════════════════════════════════════════════════════════════════════════════════\n";
}


template<typename Key>

class DynamicGraph{
	
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

	void increaseIncidentToReserveNodes(Forest<Key> * maxLevelForest, Key u, Key v){
		
		Tree<Key> * uTree = maxLevelForest->getTreeContaining(u);
		Tree<Key> * vTree = maxLevelForest->getTreeContaining(v);

		Node<Key> * uu = maxLevelForest->getNode(u);
		Node<Key> * vv = maxLevelForest->getNode(v);

		if(!uu->isIncidentToReserveNode){
			uu->isIncidentToReserveNode = true;
			uTree->splay(uu);
		}

		if(!vv->isIncidentToReserveNode){
			vv->isIncidentToReserveNode = true;
			vTree->splay(vv);
		}
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
			
		for(unsigned int i = nodeLevel; i <= this->maxLevel; ++i){
			
			Tree<Key> * uTree = this->forests[i]->getTreeContaining(u);
			Tree<Key> * treeV = this->forests[i]->getTreeContaining(v);
			
			if(uTree->size() > treeV->size()){
				std::swap(uTree, treeV);
				std::swap(u, v);
			} 
			
			while(uTree->root->nodesAtLevel > 0){
				decreaseNodesLevel(uTree, i);
			}
				
				bool nodeIsReplaced = false;
				
			while(uTree->root->reserveNodes > 0 && !nodeIsReplaced){
				
				Node<Key> * xVertex = uTree->getReserveNode(uTree->root);
				xVertex->isIncidentToReserveNode = false;
				xVertex->setReserveNodesCount();
				std::vector<Key> reserveNodesToBeRemoved;
				Key x = xVertex->first;

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
				
				for (const Key & y : this->adjacencyLists[i]->adjList[x]) {
					this->adjacencyLists[i]->remove(x, y);
					this->forests[i]->decreaseIncidentToReserveNodeCount(y);
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
			
			for(unsigned int i = 0; i <= this->maxLevel; ++i){
				adjacencyLists[i] = new AdjacencyList<Key>();
				forests[i] = new Forest<Key>(vertices);
			}
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// add node (u, v) in O(lg(n))
		void add(Key u, Key v){

			updateMapNodeLevels(u, v, this->maxLevel);
			Forest<Key> * maxLevelForest = this->forests[this->maxLevel];

			if(maxLevelForest->isConnected(u, v)){
				this->adjacencyLists[this->maxLevel]->add(u, v);

				increaseIncidentToReserveNodes(maxLevelForest, u, v);
			} 
			else{
				maxLevelForest->link(u, v);
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