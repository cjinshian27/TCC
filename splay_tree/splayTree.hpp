
#include "node.hpp"

template <typename Key>

//splay tree class
class SplayTree{

	private:	

		//get the size of an node
		unsigned int size(Node<Key> * node){
			if(!node){
				return 0;
			}
			return node->size;
		}

		//delete the whole splay tree
		void stopMemoryLeak(Node<Key> * node){
			if (!node){
	    		return;
			}
	
			stopMemoryLeak(node->left);
			stopMemoryLeak(node->right);
			delete node;
			node = nullptr;
		}
		
		//get the maximum key from the left subtree
		Node<Key> * maxLeftSubtree(Node<Key> * node){
			Node<Key> * current = node;
			while(current->right){
				current = current->right;
			}
			return current;
		}

		//get the minimum key from the right subtree
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
		 	if(nodeRightLeft != nullptr){
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
			if(nodeLeftRight != nullptr){
				nodeLeftRight->parent = node;
			}
			nodeLeft->parent = dad;

			return nodeLeft;
		}
		
		//print out the splay tree
		void printTree(Node<Key> * node){
			if(node == nullptr){
				return;
			}
			
			printTree(node->left);
			
			node->print();

			printTree(node->right);
		}

		//do a splay operation on given node 
		void splay(Node<Key> * node){
			while(node->parent != nullptr){
				if(node->parent->parent == nullptr){
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
										
					if(grandGrandParent != nullptr){
						if(grandGrandParent->right == grandpa){
							grandGrandParent->right = node;
						}
						else{
							grandGrandParent->left = node;
						}
						grandGrandParent->setSize();
					}
				}
			}
			this->root = node;
			
		}
		
		//operator < overloading for splay tree comparison
		bool operator < (const SplayTree & that){
			return this->root->id < that->root->id;
		}
		
		//operator > overloading for splay tree comparison
		bool operator > (const SplayTree & that){
			return this->root->id > that->root->id;
		}
		
		//operator = overloading for splay tree's comparison
		bool operator = (const SplayTree & that){
			return this->root->id == that->root->id;
		}

	public:

		//splay tree's root node
		Node<Key> * root = nullptr;
		
		//class' constructor
		SplayTree(){
		}

		//class' parameterized constructor
		SplayTree(Key u, Key v, unsigned int id){
			root = new Node<Key>(u, v, id);
		}

		//class parameterized constructor
		SplayTree(Node<Key> * node){
			root = node;
		}

		//class' destructor
		~SplayTree(){
			stopMemoryLeak(this->root);
		}

		//insert a node with given key into the splay tree
		void insert(Key key){

			if(this->root == nullptr){
				this->root = new Node<Key>(key);
				return;
			}

			Node<Key> * current = this->root;
			
			while(current != nullptr){
				if(key > current->key){
					if(current->right == nullptr){
						Node<Key> * node = new Node<Key>(key);
						current->right = node;
						node->parent = current;
						splay(node);
						return;
					}
					else{
						current = current->right;
					}
				}
				else if(key < current->key){
					if(current->left == nullptr){
						Node<Key> * node = new Node<Key>(key);
						current->left = node; 
						node->parent = current;
						splay(node);
						return;
					}
					else{
						current = current->left;
					}
				}
				else{
					splay(current);
					return;
				}
			}

		}



		//return the k-th node from node
		Node<Key> * k_th(Node<Key> * node, unsigned int k){
			unsigned int value = size(node->left) + 1;
			if(value == k){
				return node;
			}
			else if(value > k){	
				return k_th(node->left, k);
			}
			else{
				return k_th(node->right, k - value);
			}
		}

		//splay the node whose key is given and print out their subtrees
		std::pair<SplayTree *, SplayTree *> split(unsigned int k){

			if(!k){
				return {nullptr, this};
			}

			Node<Key> * aux = k_th(this->root, k);
			splay(aux);

			aux = this->root->right;
			
			if(aux != nullptr){
				aux->parent = nullptr;
			}


			this->root->right = nullptr;
			this->root->setSize();

			SplayTree<Key> * splayTree1 = new SplayTree<Key>(this->root);
			SplayTree<Key> * splayTree2 = new SplayTree<Key>(aux);
			if(aux == nullptr){
				return {splayTree1, nullptr};
			}

			return {splayTree1, splayTree2};
		}		

		//join two given trees
		void join(SplayTree * & thatSplayTree){
			
			Node<Key> * node = maxLeftSubtree(this->root);
			splay(node);

			if(thatSplayTree == nullptr){
				this->root->right = nullptr;
			}
			else{
				this->root->right = thatSplayTree->root;
				this->root->right->parent = this->root;
			}
			this->root->setSize();
		}

		//balance the tree by splaying given node
		void balance(Node<Key> * node){
			if(node == nullptr) return;
			splay(node);
		}

		//call the print function
		void print(){
			printTree(this->root);
		}
};