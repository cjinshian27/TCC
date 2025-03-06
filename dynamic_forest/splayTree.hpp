#include "edge.hpp"
template <typename Key>

class SplayTree{

	private:	


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
			
			Edge<Key> * currentEdge = edge;
			
			while(currentEdge->right){
				currentEdge = currentEdge->right;
			}
			
			return currentEdge;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//get the edge with minimum key from the right subtree
		Edge<Key> * rightSubtreeMinNode(Edge<Key> * edge){
			
			Edge<Key> * currentEdge = edge;
			
			while(currentEdge->left){
				currentEdge = currentEdge->left;
			}
			
			return currentEdge;
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
		/*
		try to find a edge with a given key and return 
		a pair of nodes, where the first edge is the 
		previous one and the second is the current one. 
		*/
		std::pair<Edge<Key> *, Edge<Key> *> find(Key key){
			
			Edge<Key> * currentEdge = this->root;
			Edge<Key> * previousEdge = nullptr;

			while(currentEdge){
				
				previousEdge = currentEdge;
				
				if(key < currentEdge->key){
					currentEdge = currentEdge->left;
				}
				else if(key > currentEdge->key){
					currentEdge = currentEdge->right;
				}
				else{
					return {currentEdge, previousEdge};
				}
			}

			return {currentEdge, previousEdge};
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//print out the splay tree
		void print(Edge<Key> * edge, unsigned int depth){
			
			if(!edge) return;
			
			print(edge->right, depth + 3);
			
			for(unsigned int i = 0; i < depth; ++i){
				std::cout << ' ';
			}

			edge->print();
	
			print(edge->left, depth + 3);
		}

	public:

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//splay tree's root edge
		Edge<Key> * root = nullptr;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		SplayTree(){
			this->root = nullptr;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		SplayTree(Key u, Key v, unsigned int id){
			this->root = new Edge<Key>(u, v, id);
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		SplayTree(Edge<Key> * edge){
			this->root = edge;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		~SplayTree(){	
			destroy(this->root);
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
		//splay the edge whose key is given and print out their subtrees
		std::pair<SplayTree *, SplayTree *> split(unsigned int k){

			if(k < 1) return {nullptr, this};

			Edge<Key> * aux = k_th(this->root, k);
			splay(aux);

			aux = this->root->right;

			if(aux) aux->parent = nullptr;

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
		void join(SplayTree * thatSplayTree){
			
			Edge<Key> * edge = leftSubtreeMaxNode(this->root);
			splay(edge);

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