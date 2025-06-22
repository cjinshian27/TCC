#include <utility>

template<typename Key>

class Node{
	public:
	
		// if the node is (u, v), u != v, tells if (u, v) is of level i (from the forest)
		bool isLevel; 
		
		// if the node is (u, u), tells if a reserve edge of level i is incident to (u, u) 
		bool isIncidentToReserveNode;
		
		unsigned int id;
		unsigned int size;

		// counts the number of nodes in the subtree where isLevel = true
		unsigned int nodesAtLevel;

		// counts the number of nodes in the subtree where isIncidentToReserveNode = true
		unsigned int reserveNodes;
			Key first;
			Key second;
			
			Node * left = nullptr;
			Node * right = nullptr;
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


		// count the number of nodes that are level i
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

		//set the node size
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

		void updateValues(){
			setSize();
			setNodeLevelCount();
			setReserveNodesCount();
		}
};