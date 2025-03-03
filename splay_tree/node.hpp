template <typename Key>

class Node{
	
	public:
		Node * parent;
		Node * right;
		Node * left;
		Key key;

		//node's constructor
		Node(Key key){
			
			this->key = key;
			parent = nullptr;
			right = nullptr;
			left = nullptr;
		}	
};