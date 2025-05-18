#include <utility>

template<typename Key>

class Edge{

	private:
		Key u;
		Key v;
		
	public:
	
		bool isLevel;
		bool isIncidentToReserveEdge;
		unsigned int id;
		unsigned int size;
		unsigned int edgesAtLevel;
		unsigned int reserveEdgesAtLevel;
		std::pair<Key, Key> edge;
		
		Edge * left = nullptr;
		Edge * right = nullptr;
		Edge * parent = nullptr;

		Edge(Key first, Key second, unsigned int id){

			this->edge.first = first;
			this->edge.second = second;
			this->id = id;
			this->size = 1;
			
			this->isLevel = false;
			this->edgesAtLevel = 0;
			
			this->isIncidentToReserveEdge = false; 
			this->reserveEdges = 0;
		}

		//print the edge in the (u, v) format
		void print(){

			std::cout << "(" << this->edge.first << " " << this->edge.second << ")" << '\n';
		}

		void setReserveEdgesCount(){
			
			unsigned int leftSubtreeReserveEdges = 0;
			unsigned int rightSubtreeReserveEdges = 0;

			if(this->left){
				if(this->left->isIncidentToReserveEdge){
					leftSubtreeReserveEdges = this->left->reserveEdges;
				}
			}

			if(this->right){
				if(this->right->isIncidentToReserveEdge){
					rightSubtreeReserveEdges = this->right->reserveEdges;
				}
			}
			
			this->reserveEdges = this->isIncidentToReserveEdge ? 1 : 0;

			this->reserveEdges += leftSubtreeReserveEdges + rightSubtreeReserveEdges;
		}


		// count the number of edges that are level i
		void setEdgeLevelCount(){
			
			unsigned int leftSubtreeEdgesAtLevel = 0;
			unsigned int rightSubtreeEdgesAtLevel = 0;

			if(this->left){
				if(this->left->isLevel){
					leftSubtreeEdgesAtLevel = this->left->edgesAtLevel;
				}
			}

			if(this->right){
				if(this->right->isLevel){
					rightSubtreeEdgesAtLevel = this->right->edgesAtLevel;
				}
			}
			
			this->edgesAtLevel = this->isLevel ? 1 : 0;

			this->edgesAtLevel += leftSubtreeEdgesAtLevel + rightSubtreeEdgesAtLevel;
		}

		//set the edge size
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
};