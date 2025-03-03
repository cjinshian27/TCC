
template <typename Key>

//splay tree class
class SplayTree{

	private:	
		//splay tree's root node
		Node<Key> * root;
		
		//delete the whole splay tree
		void destroy(Node<Key> * node){
			if (!node){
	    		return;
			}
	
			destroy(node->left);
			destroy(node->right);
			delete node;
			node = nullptr;
		}
		
		//get the node with maximum key from the left subtree
		Node<Key> * maxLeftSubtree(Node<Key> * node){
			
			Node<Key> * currentNode = node;
			
			while(currentNode->right){
				currentNode = currentNode->right;
			}
			
			return currentNode;
		}

		//get the node with minimum key from the right subtree
		Node<Key> * minRightSubtree(Node<Key> * node){
			Node<Key> * current = node;
			
			while(current->left){
				current = current->left;
			}
			
			return current;
		}
		
		//rotate left to balance the subtree
		Node<Key> * leftRotate(Node<Key> * node){

	 	   	Node<Key> * nodeRight = node->right;
	 	   	Node<Key> * dad = node->parent;

			Node<Key> * nodeRightLeft = node->right->left;

		   	nodeRight->left = node;
		   	node->parent = nodeRight;
		   	
		 	node->right = nodeRightLeft;
		 	if(nodeRightLeft){
		 		nodeRightLeft->parent = node;
		 	}
		 	nodeRight->parent = dad;

		 	return nodeRight;
		}

		//rotate right to balance the subtree
		Node<Key> * rightRotate(Node<Key> * node){
			Node<Key> * nodeLeft = node->left;
	 	   	Node<Key> * dad = node->parent;

			Node<Key> * nodeLeftRight = node->left->right;

			nodeLeft->right = node;
			node->parent = nodeLeft;

			node->left = nodeLeftRight;
			if(nodeLeftRight){
				nodeLeftRight->parent = node;
			}
			nodeLeft->parent = dad;

			return nodeLeft;
		}

		//do a splay operation on given node 
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
					Node<Key> * grandpa = node->parent->parent;
					Node<Key> * aux;
					if(node == node->parent->left && node->parent == node->parent->parent->left){
						aux = rightRotate(node->parent->parent);
						node = rightRotate(aux);
						node->parent = grandGrandParent;

					}
					else if(node == node->parent->right && node->parent == node->parent->parent->right){
						
						aux = leftRotate(node->parent->parent);
						node = leftRotate(aux);
						node->parent = grandGrandParent;
	
					}	
					else if(node == node->parent->right && node->parent == node->parent->parent->left){						
						aux = leftRotate(node->parent);
						aux->parent->left = aux;
						node = rightRotate(aux->parent);
						node->parent = grandGrandParent;
					}
					else{
						aux = rightRotate(node->parent);
						aux->parent->right = aux;
						node = leftRotate(aux->parent);
						node->parent = grandGrandParent;
					}

					if(grandGrandParent){
						if(grandGrandParent->right == grandpa){
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

		//print out the splay tree
		void print(Node<Key> * node, unsigned int depth){
			if(!node){
				return;
			}
			
			print(node->left, depth + 3);
			
			for(unsigned int i = 0; i < depth; ++i){
				std::cout << ' ';
			}
			std::cout << node->key << '\n';
			
			print(node->right, depth + 3);
		}
	public:
		
		//class' constructor
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

		//search for a node with given key on the splay tree
		bool search(Key key){
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
					splay(currentNode);
					return true;
				}
			}

			if(previousNode){
				splay(previous);
			}
			return false;
		}

		//remove a node with given key from the splay tree
		void remove(Key key){
			Node<Key> * currentNode = this->root;
			Node<Key> * previousNode = nullptr;

			//find the node with the key first
			while(currentNode){
				
				previousNode = currentNode;
				
				if(key < currentNode->key){
					currentNode = currentNode->left;
				}
				else if(key > currentNode->key){
					currentNode = currentNode->right;
				}
				else{
					break;
				}
			}

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
							currentNode = nullptr
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
					
					Node<Key> * maxLS = maxLeftSubtree(currentNode->left);
					Node<Key> * parent = maxLS->parent;
					currentNode->setKey(maxLS->key);
					
					if(parent->right == maxLS){
						delete parent->right;
						parent->right = nullptr;
					}
					else{
						delete parent->left;
						parent->left = nullptr;
					}

					splay(parent);
				}
				else if(currentNode->left){
					
					Node<Key> * maxLS = maxLeftSubtree(current->left);
					Node<Key> * parent = maxLS->parent;
					currentNode->setKey(maxLS->key);
					
					if(parent->right == maxLS){
						delete parent->right;
						parent->right = nullptr;
					}
					else{
						delete parent->left;
						parent->left = nullptr;
					}

					splay(parent);
				}
				else{
					
					Node<Key> * minRS = minRightSubtree(currentNode->right);
					Node<Key> * parent = minRS->parent;	
					current->setKey(minRS->key);

					if(parent->right == minRS){
						delete parent->right;
						parent->right = nullptr;
					}
					else{
						delete parent->left;
						parent->left = nullptr;
					}

					splay(parent);
				}

			}

			//if node is not found, do a splay on the previous visited node
			else if(previous){
				splay(previous);
			}
		}

		//return the node with the smallest key 
		Key min(){
			Node<Key> * current = this->root;
			while(current->left){
				current = current->left;
			}
			splay(current);
			return current->key;
		}

		//splay the node whose key is given and print out their subtrees
		std::pair<SplayTree *, SplayTree *> split(Key key){
			
			if(!this->root){
				return {nullptr, nullptr};
			}
			
			Node<Key> * current = this->root;
			Node<Key> * previous = nullptr;

			while(current){
				previous = current;
				if(key < current->key){
					current = current->left;
				}
				else if(key > current->key){
					current = current->right;
				}
				else{
					break;
				}
			}

			if(current){
				splay(current);
			}
			else{
				splay(previous);
			}
			Node<Key> * aux = this->root->right;
			this->root->right = nullptr;

			SplayTree * splayTree1 = new SplayTree(this->root);
			SplayTree * splayTree2 = new SplayTree(aux);

			return {splayTree1, splayTree2};
		}		

		//join two given trees
		void join(SplayTree * thatSplayTree){
			Node<Key> * node = maxLeftSubtree(this->root);
			splay(node);
			node->right = thatSplayTree->root;
		}

		//call the print function
		void print(){
			print(this->root, 0);
		}
};