#include <utility>

template<typename Key>

class Node{
	
	public:

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
		
		// keeps the left end of an edge
		Key first;

		// keeps the right end of an edge
		Key second;
		
		// left child node
		Node * left = nullptr;

		// right child node
		Node * right = nullptr;

		// parent node
		Node * parent = nullptr;

		Node(Key first, Key second, unsigned int id){

				this->first = first;
				this->second = second;
				this->id = id;
				this->size = 1;
				
				this->isLevel = false;
				this->nodesAtLevel = 0;
				
				this->isIncidentToReserveNode = false; 
				this->reserveNodes = 0;
			}

		//print the node in the (u, v) format
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
		
		// update the node attributes
		void updateValues(){
			setSize();
			setNodeLevelCount();
			setReserveNodesCount();
		}
};