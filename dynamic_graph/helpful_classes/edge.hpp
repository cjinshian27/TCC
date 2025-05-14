#include <utility>
#include <iostream>

template<typename Key>

class Edge{

	private:
		Key u;
		Key v;
		
	public:
	
		unsigned int id;
		unsigned int size;
		unsigned int level;
		unsigned int edgesAtLevel_i;
		std::pair<Key, Key> edge;
		
		Edge * left = nullptr;
		Edge * right = nullptr;
		Edge * parent = nullptr;

		Edge(Key first, Key second, unsigned int id){

			this->edge.first = first;
			this->edge.second = second;
			this->id = id;
			this->size = 1;
			this->edgesAtLevel_i = 0;
			this->level = 0;
		}

		//print the edge in the (u, v) format
		void print(){

			std::cout << "(" << this->edge.first << " " << this->edge.second << ")" << '\n';
		}

		// count the number of edges that are level i
		void countEdgesAtLevel(unsigned int i){
			
			unsigned int leftSubtreeEdgesAtLevel_i = 0;
			unsigned int rightSubtreeEdgesAtLevel_i = 0;

			if(this->left){
				if(this->left->isLevel(i)){
					leftSubtreeEdgesAtLevel_i = this->left->edgesAtLevel_i;
				}
			}

			if(this->right){
				if(this->right->isLevel(i)){
					rightSubtreeEdgesAtLevel_i = this->right->edgesAtLevel_i;
				}
			}
			
			if(this->isLevel(i)){
				this->edgesAtLevel_i++;
			}

			this->edgesAtLevel_i += leftSubtreeEdgesAtLevel_i + rightSubtreeEdgesAtLevel_i;
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

		void setLevel(unsigned int level){
			this->level = level;
		}

		unsigned int getLevel(){
			return this->level;
		}

		bool isLevel(unsigned int level){
			return this->level == level;
		}

		std::pair<Key, Key> getEnds(){
			return {this->u, this->v};
		}
}