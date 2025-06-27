template<typename Key>

class Edge{
	
	public:

		unsigned int id;
		unsigned int size;
		
		Key first;
		Key second;

		Edge * left = nullptr;
		Edge * right = nullptr;
		Edge * parent = nullptr;

		Edge(Key first, Key second, unsigned int id){

			this->first = first;
			this->second = second;
			this->id = id;
			this->size = 1;
		}

		//print the edge in the (u, v) format
		void print(){

			std::cout << "(" << this->first << " " << this->second << ")" << '\n';
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