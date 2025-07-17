#include <climits>
#include <utility>
#include "minHeap.hpp"

template<typename Key>

class Node{
	
	public:

		// node's weight
		int weight;

		/*
		keeps track of the node (u, v) with the smallest weight 
		from the entire forest, since we want to build a minimum 
		spanning forest
		*/ 
		int minWeight;

		// stores a list of neighbors with its respective weights
		MinHeap<Key> * adjacencyList;

		// if the node is (u, v), u != v, tells if (u, v) is of forest level i
		bool isLevel; 

		// if the node is (u, u), tells if a reserve edge of level i is incident to (u, u) 
		bool isIncidentToReserveNode;
		
		// counts the number of nodes in the subtree where isLevel = true
		unsigned int nodesAtLevel;
		
		// counts the number of nodes in the subtree where isIncidentToReserveNode = true
		unsigned int reserveNodes;

		// identifies each node uniquely
		unsigned int id;

		// keeps the size in the subtree
		unsigned int size;
		
		// keeps the left end of a node
		Key first;

		// keeps the right end of a node
		Key second;
		
		// left child node
		Node * left = nullptr;

		// right child node
		Node * right = nullptr;

		// parent node
		Node * parent = nullptr;

		Node(Key first, Key second, unsigned int id, int weight){

			this->first = first;
			this->second = second;
			this->id = id;
			this->size = 1;
			
			this->isLevel = false;
			this->nodesAtLevel = 0;
			
			this->isIncidentToReserveNode = false; 
			this->reserveNodes = 0;
			this->adjacencyList = new MinHeap<Key>();
			this->weight = weight;
		}

		//prints the node in the (u, v) format
		void print(){

			std::cout << "(" << this->first << " "<< this->second 
			          << " | " << isLevel << ":" << nodesAtLevel << " " 
					  << isIncidentToReserveNode << ":" << reserveNodes << 
					  " - size: " << size << ")" << '\n';
		}
		
		// updates the reserveNodes attribute count
		void setReserveNodesCount(){
			
			unsigned int leftSubtreeReserveNodes = 0;
			unsigned int rightSubtreeReserveNodes = 0;

			if(this->left){
				leftSubtreeReserveNodes = this->left->reserveNodes;
			}

			if(this->right){
				rightSubtreeReserveNodes = this->right->reserveNodes;
			}
			
			this->reserveNodes = this->isIncidentToReserveNode ? 1 : 0;
			
			this->reserveNodes += leftSubtreeReserveNodes + rightSubtreeReserveNodes;
		}

		// updates the nodesAtLevel attribute count
		void setNodeLevelCount(){
			
			unsigned int leftSubtreeNodesAtLevel = 0;
			unsigned int rightSubtreeNodesAtLevel = 0;

			if(this->left){
				leftSubtreeNodesAtLevel = this->left->nodesAtLevel;
			}

			if(this->right){
				rightSubtreeNodesAtLevel = this->right->nodesAtLevel;
			}
			
			this->nodesAtLevel = this->isLevel ? 1 : 0;

			this->nodesAtLevel += leftSubtreeNodesAtLevel + rightSubtreeNodesAtLevel;
		}

		// updates the size attribute count
		void setSize(){

			if(this->left && this->right){
				this->size = 1 + this->left->size + this->right->size;
			}
			else if(this->left){
				this->size = 1 + this->left->size;
			}
			else if(this->right){
				this->size = 1 + this->right->size;
			}
			else{
				this->size = 1;
			}
		}
		
		// updates the minWeight attribute
		void setMinWeight(){
			int currentMinWeight = INT_MAX;

			if(this->left && this->left->minWeight < currentMinWeight){
				currentMinWeight = this->left->minWeight;
			}

			if(this->right && this->right->minWeight < currentMinWeight){
				currentMinWeight = this->right->minWeight;
			}
			
			if(!this->adjacencyList->empty()){
				std::pair<Key, int> neighbor = this->adjacencyList->getMin();
				if(neighbor.second < currentMinWeight){	
					currentMinWeight = neighbor.second;
				}  
			}

			this->minWeight = currentMinWeight;
		}

		void addNeighbor(Key neighbor, int weight){
			this->adjacencyList->insert(neighbor, weight);
		}

		void removeNeighbor(Key neighbor){
			this->adjacencyList->remove(neighbor);
		}

		// updates the node attributes
		void updateValues(){
			setSize();
			setMinWeight();
			setNodeLevelCount();
			setReserveNodesCount();
		}
};