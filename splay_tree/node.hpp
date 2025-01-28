template <typename Key>

class Node{
	
	public:
		Node * parent;
		Node * right;
		Node * left;
		Key key;

		//key setter
		void setKey(Key key){
			
			this->key = key;
		}

		//node's constructor
		Node(Key key){
			
			this->key = key;
			parent = nullptr;
			right = nullptr;
			left = nullptr;
		}	
};