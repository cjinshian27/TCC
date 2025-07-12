#include "node.hpp"
template <typename Key>

class SplayTree{

	private:	


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//get the size of an node
		unsigned int size(Node<Key> * node){

			if(!node) return 0;
			return node->size;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		//delete the whole splay tree
		void destroy(Node<Key> * node){
			
			if (!node) return;
	
			destroy(node->left);
			destroy(node->right);

			delete node;
			node = nullptr;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		//get the node with maximum key from the left subtree
		Node<Key> * leftSubtreeMaxNode(Node<Key> * node){
			
			Node<Key> * currentNode = node;
			
			while(currentNode->right){
				currentNode = currentNode->right;
			}
			
			return currentNode;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//get the node with minimum key from the right subtree
		Node<Key> * rightSubtreeMinNode(Node<Key> * node){
			
			Node<Key> * currentNode = node;
			
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
			
			y->setSize();
			x->parent = parent;

			return x;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//print out the splay tree
		void print(Node<Key> * node, unsigned int depth){
			
			if(!node) return;
			
			print(node->left, depth + 3);
			
			for(unsigned int i = 0; i < depth; ++i){
				std::cout << ' ';
			}

			node->print();
	
			print(node->right, depth + 3);
		}

	public:

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//splay tree's root node
		Node<Key> * root = nullptr;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		SplayTree(){
			this->root = nullptr;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		SplayTree(Key u, Key v, unsigned int id){
			this->root = new Node<Key>(u, v, id);
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		SplayTree(Node<Key> * node){
			this->root = node;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		~SplayTree(){	
			destroy(this->root);
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

					node->setSize();

					if(grandGrandParent){
						if(grandGrandParent->right == grandparent){
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
			this->root->setSize();
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//return the k-th node
		Node<Key> * k_th(Node<Key> * node, unsigned int k){
			
			unsigned int position = size(node->left) + 1;
			
			if(position == k){
				return node;
			}
			else if(position > k){	
				return k_th(node->left, k);
			}
			else{
				return k_th(node->right, k - position);
			}
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//splay the node whose key is given and print out their subtrees
		std::pair<SplayTree *, SplayTree *> split(unsigned int k){

			if(k < 1) return {nullptr, this};

			Node<Key> * aux = k_th(this->root, k);
			splay(aux);

			aux = this->root->right;

			if(aux) aux->parent = nullptr;

			this->root->right = nullptr;
			this->root->setSize();

			SplayTree<Key> * splayTree1 = new SplayTree<Key>(this->root);

			if(!aux) return {splayTree1, nullptr};
			
			SplayTree<Key> * splayTree2 = new SplayTree<Key>(aux);

			return {splayTree1, splayTree2};
		}		

		//join two given trees
		void join(SplayTree * thatSplayTree){
			
			Node<Key> * node = leftSubtreeMaxNode(this->root);
			splay(node);

			if(!thatSplayTree){
				this->root->right = nullptr;
			}
			else{
				this->root->right = thatSplayTree->root;
				this->root->right->parent = this->root;
			}
			this->root->setSize();
		}

		//call the print function
		void print(){
			print(this->root, 0);
		}

};