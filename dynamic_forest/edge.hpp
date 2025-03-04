template<typename Key>

class Edge{
	
	public:

		unsigned int id;
		std::pair<Key, Key> edge;
		unsigned int size;
		
		Edge * left = nullptr;
		Edge * right = nullptr;
		Edge * parent = nullptr;

		Edge(Key first, Key second, unsigned int id){

			this->edge.first = first;
			this->edge.second = second;
			this->id = id;
			this->size = 1;
		}

		//print the edge in the (u, v) format
		void print(){

			std::cout << this->edge.first << " " << this->edge.second << '\n';
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