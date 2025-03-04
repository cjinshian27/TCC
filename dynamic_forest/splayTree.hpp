
template <typename Key>

class SplayTree{

	private:	

		//splay tree's root node
		Node<Key> * root;
		
		//delete the whole splay tree
		void destroy(Node<Key> * node){
			
			if (!node) return;
	
			destroy(node->left);
			destroy(node->right);

			delete node;
			node = nullptr;
		}
		
		//get the node with maximum key from the left subtree
		Node<Key> * leftSubtreeMaxNode(Node<Key> * node){
			
			Node<Key> * currentNode = node;
			
			while(currentNode->right){
				currentNode = currentNode->right;
			}
			
			return currentNode;
		}

		//get the node with minimum key from the right subtree
		Node<Key> * rightSubtreeMinNode(Node<Key> * node){
			
			Node<Key> * currentNode = node;
			
			while(currentNode->left){
				currentNode = currentNode->left;
			}
			
			return currentNode;
		}
		
		/*

		left rotate to balance the subtree
			
			  y                x  
		     / \              / \
		    C   x    ===>    y   A
		       / \ 			/ \
			  B   A        C   B
  		
		*/
		Node<Key> * leftRotate(Node<Key> * y){

	 	   	Node<Key> * x = y->right;
	 	   	Node<Key> * parent = y->parent;

			Node<Key> * B = x->left;

		   	x->left = y;
		   	
		   	y->parent = x;
		 	y->right = B;
		 	
		 	if(B){
		 		B->parent = y;
		 	}
		 	
		 	x->parent = parent;

		 	return x;
		}

		/*
		right rotate to balance the subtree
			
				y                x
			   / \				/ \
			  x   C    ===>    A   y
			 / \                  / \
			A   B                B   C

		*/
		Node<Key> * rightRotate(Node<Key> * y){
			
			Node<Key> * x = y->left;
	 	   	Node<Key> * parent = y->parent;

			Node<Key> * B = x->right;

			x->right = y;
			y->parent = x;

			y->left = B;
			
			if(B){
				B->parent = y;
			}
			
			x->parent = parent;

			return x;
		}

		//do a splay operation on a given node 
		void splay(Node<Key> * node){

			while(node->parent){
				
				if(!node->parent->parent){
					
					if(node == node->parent->left){
						node = rightRotate(node->parent);
					}
					else{
						node = leftRotate(node->parent);
					}
				}
				else{
					
					Node<Key> * grandGrandParent = node->parent->parent->parent;
					Node<Key> * grandparent = node->parent->parent;
					Node<Key> * aux;
					
					//right right rotate
					if(node == node->parent->left && node->parent == node->parent->parent->left){
						aux = rightRotate(node->parent->parent);
						node = rightRotate(aux);
					}

					//left left rotate
					else if(node == node->parent->right && node->parent == node->parent->parent->right){
						aux = leftRotate(node->parent->parent);
						node = leftRotate(aux);
					}	

					//left right rotate
					else if(node == node->parent->right && node->parent == node->parent->parent->left){						
						aux = leftRotate(node->parent);
						aux->parent->left = aux;
						node = rightRotate(aux->parent);
					}

					//right left rotate
					else{
						aux = rightRotate(node->parent);
						aux->parent->right = aux;
						node = leftRotate(aux->parent);
					}

					if(grandGrandParent){
						if(grandGrandParent->right == grandparent){
							grandGrandParent->right = node;
						}
						else{
							grandGrandParent->left = node;
						}
					}

				}
			}

			this->root = node;
		}

		/*
		try to find a node with a given key and return 
		a pair of nodes, where the first node is the 
		previous one and the second is the current one. 
		*/
		std::pair<Node<Key> *, Node<Key> *> find(Key key){
			
			Node<Key> * currentNode = this->root;
			Node<Key> * previousNode = nullptr;

			while(currentNode){
				
				previousNode = currentNode;
				
				if(key < currentNode->key){
					currentNode = currentNode->left;
				}
				else if(key > currentNode->key){
					currentNode = currentNode->right;
				}
				else{
					return {currentNode, previousNode};
				}
			}

			return {currentNode, previousNode};
		}

		//print out the splay tree
		void print(Node<Key> * node, unsigned int depth){
			
			if(!node) return;
			
			print(node->right, depth + 3);
			
			for(unsigned int i = 0; i < depth; ++i){
				std::cout << ' ';
			}

			std::cout << node->key << " ("; 

			if(node->parent){
				std::cout << node->parent->key;
			} 
			else{
				std::cout << "null";
			}

			std::cout << ":";

			if(node->left){
				std::cout << node->left->key;
			}
			else{
				std::cout << "null";
			}

			std::cout << ":";
			
			if(node->right){
				std::cout << node->right->key;
			}
			else{
				std::cout << "null";
			}

			std::cout << ")\n'";

			print(node->left, depth + 3);
		}

	public:
		
		SplayTree(){
			this->root = nullptr;
		}

		SplayTree(Node<Key> * node){
			this->root = node;
		}


		~SplayTree(){	
			destroy(this->root);
		}

		//insert a node with a given key into the splay tree
		void insert(Key key){

			if(!this->root){
				this->root = new Node<Key>(key);
				return;
			}

			Node<Key> * currentNode = this->root;
			
			while(currentNode != nullptr){
				
				if(key > currentNode->key){
					
					if(!currentNode->right){
						
						Node<Key> * newNode = new Node<Key>(key);
						currentNode->right = newNode;
						newNode->parent = currentNode;
						splay(newNode);
						return;
					}
					else{
						
						currentNode = currentNode->right;
					}
				}
				else if(key < currentNode->key){
					
					if(!currentNode->left){
						
						Node<Key> * newNode = new Node<Key>(key);
						currentNode->left = newNode; 
						newNode->parent = currentNode;
						splay(newNode);
						return;
					}
					else{
						
						currentNode = currentNode->left;
					}
				}
				else{
					
					splay(currentNode);
					return;
				}
			}

		}

		/*
		search for a node with given key on the splay, 
		return true if found and false otherwise.
		*/
		bool search(Key key){

			std::pair<Node<Key> *, Node<Key> *> pairOfNodes = find(key);
			Node<Key> * currentNode = pairOfNodes.first;
			Node<Key> * previousNode = pairOfNodes.second;

			if(currentNode){
				splay(currentNode);
				return true;
			}
			else if(previousNode){
				splay(previousNode);
			}
			return false;

		}

		//remove a node with given key from the splay tree
		void remove(Key key){

			std::pair<Node<Key> *, Node<Key> *> pairOfNodes = find(key);
			Node<Key> * currentNode = pairOfNodes.first;
			Node<Key> * previousNode = pairOfNodes.second;

			//if node is found, check their conditions
			if(currentNode){

				//no child
				if(!currentNode->left && !currentNode->right){
					
					Node<Key> * parent = currentNode->parent;
					
					if(parent){
						if(parent->right == currentNode){
							delete(currentNode);
							currentNode = nullptr;
							parent->right = nullptr;
						}
						else{
							delete(currentNode);
							currentNode = nullptr;
							parent->left = nullptr;
						}
						splay(parent);
					}
					else{
						delete currentNode;
						currentNode = nullptr;
						this->root = nullptr;
					}
				}
				else if(currentNode->left && currentNode->right){
					
					Node<Key> * maxNode = leftSubtreeMaxNode(currentNode->left);
					splay(maxNode);
					maxNode->right = currentNode->right;
					currentNode->right->parent = maxNode;

				}
				else if(currentNode->left){
					
					Node<Key> * maxNode = leftSubtreeMaxNode(currentNode->left);
					splay(maxNode);
					this->root->right = nullptr;
				}
				else{
					Node<Key> * minNode = rightSubtreeMinNode(currentNode->right);
					splay(minNode);
					this->root->left = nullptr;
				}
				
				delete (currentNode);
				currentNode = nullptr;

			}

			//if node is not found, do a splay on the previous visited node
			else if(previousNode){
				splay(previousNode);
			}
		}

		//return the node with the smallest key 
		Key min(){
			
			Node<Key> * currentNode = this->root;
			
			while(currentNode->left){
				currentNode = currentNode->left;
			}
			
			splay(currentNode);
			return currentNode->key;
		}

		//splay the node whose key is given and print out their subtrees
		std::pair<SplayTree *, SplayTree *> split(Key key){
			
			if(!this->root){
				return {nullptr, nullptr};
			}
			
			std::pair<Node<Key> *, Node<Key> *> pairOfNodes = find(key);
			Node<Key> * currentNode = pairOfNodes.first;
			Node<Key> * previousNode = pairOfNodes.second;

			if(currentNode){
				splay(currentNode);
			}
			else if(previousNode){
				splay(previousNode);
			}

			Node<Key> * aux = this->root->right;
			this->root->right = nullptr;

			SplayTree * splayTree1 = new SplayTree(this->root);
			SplayTree * splayTree2 = new SplayTree(aux);

			return {splayTree1, splayTree2};
		}		

		//join two given trees
		void join(SplayTree * thatSplayTree){
			
			Node<Key> * node = leftSubtreeMaxNode(this->root);
			splay(node);
			node->right = thatSplayTree->root;
		}

		//call the print function
		void print(){
			print(this->root, 0);
		}
};