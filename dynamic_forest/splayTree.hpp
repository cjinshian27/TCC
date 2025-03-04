
template <typename Key>

class SplayTree{

	private:	
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//splay tree's root edge
		Edge<Key> * root;


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//get the size of an edge
		unsigned int size(Edge<Key> * edge){

			if(!edge) return 0;
			return edge->size;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		//delete the whole splay tree
		void destroy(Edge<Key> * edge){
			
			if (!edge) return;
	
			destroy(edge->left);
			destroy(edge->right);

			delete edge;
			edge = nullptr;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		//get the edge with maximum key from the left subtree
		Edge<Key> * leftSubtreeMaxNode(Edge<Key> * edge){
			
			Edge<Key> * currentNode = edge;
			
			while(currentNode->right){
				currentNode = currentNode->right;
			}
			
			return currentNode;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//get the edge with minimum key from the right subtree
		Edge<Key> * rightSubtreeMinNode(Edge<Key> * edge){
			
			Edge<Key> * currentNode = edge;
			
			while(currentNode->left){
				currentNode = currentNode->left;
			}
			
			return currentNode;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		/*

		left rotate to balance the subtree
			
			  y                x  
		     / \              / \
		    C   x    ===>    y   A
		       / \ 			/ \
			  B   A        C   B
  		
		*/
		Edge<Key> * leftRotate(Edge<Key> * y){

	 	   	Edge<Key> * x = y->right;
	 	   	Edge<Key> * parent = y->parent;

			Edge<Key> * B = x->left;

		   	x->left = y;
		   	
		   	y->parent = x;
		 	y->right = B;
		 	
		 	if(B){
		 		B->parent = y;
		 	}
		 	
		 	y->setSize();
		 	x->parent = parent;

		 	return x;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*
		right rotate to balance the subtree
			
				y                x
			   / \				/ \
			  x   C    ===>    A   y
			 / \                  / \
			A   B                B   C

		*/
		Edge<Key> * rightRotate(Edge<Key> * y){
			
			Edge<Key> * x = y->left;
	 	   	Edge<Key> * parent = y->parent;

			Edge<Key> * B = x->right;

			x->right = y;
			y->parent = x;

			y->left = B;
			
			if(B){
				B->parent = y;
			}
			
			y->setSize();
			x->parent = parent;

			return x;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//do a splay operation on a given edge 
		void splay(Edge<Key> * edge){

			while(edge->parent){
				
				if(!edge->parent->parent){
					
					if(edge == edge->parent->left){
						edge = rightRotate(edge->parent);
					}
					else{
						edge = leftRotate(edge->parent);
					}
				}
				else{
					
					Edge<Key> * grandGrandParent = edge->parent->parent->parent;
					Edge<Key> * grandparent = edge->parent->parent;
					Edge<Key> * aux;
					
					//right right rotate
					if(edge == edge->parent->left && edge->parent == edge->parent->parent->left){
						aux = rightRotate(edge->parent->parent);
						edge = rightRotate(aux);
					}

					//left left rotate
					else if(edge == edge->parent->right && edge->parent == edge->parent->parent->right){
						aux = leftRotate(edge->parent->parent);
						edge = leftRotate(aux);
					}	

					//left right rotate
					else if(edge == edge->parent->right && edge->parent == edge->parent->parent->left){						
						aux = leftRotate(edge->parent);
						aux->parent->left = aux;
						edge = rightRotate(aux->parent);
					}

					//right left rotate
					else{
						aux = rightRotate(edge->parent);
						aux->parent->right = aux;
						edge = leftRotate(aux->parent);
					}

					edge->setSize();

					if(grandGrandParent){
						if(grandGrandParent->right == grandparent){
							grandGrandParent->right = edge;
						}
						else{
							grandGrandParent->left = edge;
						}
						grandGrandParent->setSize();
					}

				}
			}

			this->root = edge;
			this->root->setSize();
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*
		try to find a edge with a given key and return 
		a pair of nodes, where the first edge is the 
		previous one and the second is the current one. 
		*/
		std::pair<Edge<Key> *, Edge<Key> *> find(Key key){
			
			Edge<Key> * currentNode = this->root;
			Edge<Key> * previousNode = nullptr;

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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//print out the splay tree
		void print(Edge<Key> * edge, unsigned int depth){
			
			if(!edge) return;
			
			print(edge->right, depth + 3);
			
			for(unsigned int i = 0; i < depth; ++i){
				std::cout << ' ';
			}

			std::cout << edge->key << " ("; 

			if(edge->parent){
				std::cout << edge->parent->key;
			} 
			else{
				std::cout << "null";
			}

			std::cout << ":";

			if(edge->left){
				std::cout << edge->left->key;
			}
			else{
				std::cout << "null";
			}

			std::cout << ":";
			
			if(edge->right){
				std::cout << edge->right->key;
			}
			else{
				std::cout << "null";
			}

			std::cout << ")\n'";

			print(edge->left, depth + 3);
		}

	public:

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		SplayTree(){
			this->root = nullptr;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		SplayTree(Key u, Key v, unsigned int id){
			this->root = new Edge<Key>(u, v, id);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		~SplayTree(){	
			destroy(this->root);
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//return the k-th edge
		Edge<Key> * k_th(Edge<Key> * edge, unsigned int k){
			
			unsigned int position = size(edge->left) + 1;
			
			if(position == k){
				return edge;
			}
			else if(position > k){	
				return k_th(edge->left, k);
			}
			else{
				return k_th(edge->right, k - position);
			}
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//insert a edge with a given key into the splay tree
		void insert(Key key){

			if(!this->root){
				this->root = new Edge<Key>(key);
				return;
			}

			Edge<Key> * currentNode = this->root;
			
			while(currentNode != nullptr){
				
				if(key > currentNode->key){
					
					if(!currentNode->right){
						
						Edge<Key> * newNode = new Edge<Key>(key);
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
						
						Edge<Key> * newNode = new Edge<Key>(key);
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*
		search for a edge with given key on the splay, 
		return true if found and false otherwise.
		*/
		bool search(Key key){

			std::pair<Edge<Key> *, Edge<Key> *> pairOfNodes = find(key);
			Edge<Key> * currentNode = pairOfNodes.first;
			Edge<Key> * previousNode = pairOfNodes.second;

			if(currentNode){
				splay(currentNode);
				return true;
			}
			else if(previousNode){
				splay(previousNode);
			}
			return false;

		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//remove a edge with given key from the splay tree
		void remove(Key key){

			std::pair<Edge<Key> *, Edge<Key> *> pairOfNodes = find(key);
			Edge<Key> * currentNode = pairOfNodes.first;
			Edge<Key> * previousNode = pairOfNodes.second;

			//if edge is found, check their conditions
			if(currentNode){

				//no child
				if(!currentNode->left && !currentNode->right){
					
					Edge<Key> * parent = currentNode->parent;
					
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
					
					Edge<Key> * maxNode = leftSubtreeMaxNode(currentNode->left);
					splay(maxNode);
					maxNode->right = currentNode->right;
					currentNode->right->parent = maxNode;

				}
				else if(currentNode->left){
					
					Edge<Key> * maxNode = leftSubtreeMaxNode(currentNode->left);
					splay(maxNode);
					this->root->right = nullptr;
				}
				else{
					Edge<Key> * minNode = rightSubtreeMinNode(currentNode->right);
					splay(minNode);
					this->root->left = nullptr;
				}
				
				delete (currentNode);
				currentNode = nullptr;

			}

			//if edge is not found, do a splay on the previous visited edge
			else if(previousNode){
				splay(previousNode);
			}
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//return the edge with the smallest key 
		Key min(){
			
			Edge<Key> * currentNode = this->root;
			
			while(currentNode->left){
				currentNode = currentNode->left;
			}
			
			splay(currentNode);
			return currentNode->key;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//splay the edge whose key is given and print out their subtrees
		std::pair<SplayTree *, SplayTree *> split(Key key){
			
			if(!this->root){
				return {nullptr, nullptr};
			}
			
			std::pair<Edge<Key> *, Edge<Key> *> pairOfNodes = find(key);
			Edge<Key> * currentNode = pairOfNodes.first;
			Edge<Key> * previousNode = pairOfNodes.second;

			if(currentNode){
				splay(currentNode);
			}
			else if(previousNode){
				splay(previousNode);
			}

			Edge<Key> * aux = this->root->right;
			this->root->right = nullptr;

			SplayTree * splayTree1 = new SplayTree(this->root);
			SplayTree * splayTree2 = new SplayTree(aux);

			return {splayTree1, splayTree2};
		}		

		//join two given trees
		void join(SplayTree * thatSplayTree){
			
			Edge<Key> * edge = leftSubtreeMaxNode(this->root);
			splay(edge);
			edge->right = thatSplayTree->root;
		}

		//call the print function
		void print(){
			print(this->root, 0);
		}
};