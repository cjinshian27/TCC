#include "splayTree.hpp"

template<typename Key>

class DynamicForest{

	private:
		unsigned int id = 0;

		/*
		keep the remaining nodes in the forest in 
		a <Key, Key> format.
		*/
		std::set<std::pair<Key, Key>> nodeSet;

		/*
		map the nodes according to their two (Key) ends. 
		That means mapNodes[u][v] = Node<Key>(u, v).
		*/
		std::unordered_map<Key, std::unordered_map<Key, Node<Key> *>> mapNodes; 
		
		/*
		map the remaining splay trees in the forest, where 
		an id is used to identify each splay tree uniquely
		*/
		std::unordered_map<Key, SplayTree<Key> *> mapTrees; 

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		/*
		get the size of an node, which means the sum 
		of its left subtree, right subtree + 1. 
		*/
		unsigned int size(Node<Key> * node){
			
			if(!node) return 0;
			return node->size;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		/*
		return the id of the root whose tree contains node
		
		example: splaying [2:1] node
			
		([1:2] id1)      (swap ids)   ([1:2] id2)     (splay)    ([2:1] id1)        
		      \             ▶▶▶            \            ▶▶▶        /
		  	 ([2:1] id2)                  ([2:1] id1)         ([1:2] id2)
											
		*/
		unsigned int find(Node<Key> * node){	
			
			if(!node) return 0;	
			
			Node<Key> * currentNode = node;

			while(currentNode->parent){
				currentNode = currentNode->parent;
			}

			SplayTree<Key> * aux = mapTrees[currentNode->id];

			if(aux){
				std::swap(currentNode->id, node->id);
				aux->splay(node);
				mapTrees[node->id] = aux;
			} 
			
			return node->id;
		}   

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		/*
		return the rank/order of an node in the tree
		
		example: splaying [2:1] node
			
		([1:2] id1)      (swap ids)   ([1:2] id2)     (splay)    ([2:1] id1)        
		      \             ▶▶▶            \            ▶▶▶        /
		  	 ([2:1] id2)                  ([2:1] id1)         ([1:2] id2)
		*/
		unsigned int order(Node<Key> * node){
			
			Node<Key> * currentNode = node;

			while(currentNode->parent) currentNode = currentNode->parent;

			SplayTree<Key> * aux = mapTrees[currentNode->id];
			
			if(aux){
				std::swap(currentNode->id, node->id);
				aux->splay(node);
				mapTrees[node->id] = aux;
			} 

			return size(node->left) + 1;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//bring the node that contains u to the front of the euler tour
		SplayTree<Key> * bringToFront(SplayTree<Key> * splayTree, Key u){
			
			if(!splayTree) return nullptr;
			
			unsigned int position = order(mapNodes[u][u]);
			
			mapTrees.erase(splayTree->root->id);

			std::pair<SplayTree<Key> *, SplayTree<Key> *> pairOfTrees = splayTree->split(position - 1);
			
			SplayTree<Key> * aux = pairOfTrees.second;
			aux->join(pairOfTrees.first);

			return aux;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//concatenate two splay trees
		void concatenate(SplayTree<Key> * tree1, SplayTree<Key> * tree2){
			
			if(!tree1){
				mapTrees[tree2->root->id] = tree2;
				return;
			}
			
			tree1->join(tree2);
			mapTrees[tree1->root->id] = tree1;
		}
		
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
		//concatenate four pieces to form the forest: tree1, uv, tree2, vu
		void concatenate(SplayTree<Key> * tree1, Key u, SplayTree<Key> * tree2, Key v){
			
			SplayTree<Key> * uv = new SplayTree<Key>(u, v, ++id);
			SplayTree<Key> * vu = new SplayTree<Key>(v, u, ++id);
			
			this->mapNodes[u][v] = uv->root;
			this->mapNodes[v][u] = vu->root;

			tree1->join(uv);
			tree1->join(tree2);
			tree1->join(vu);
			mapTrees[tree1->root->id] = tree1;
		}	
		
		public:	

		DynamicForest(std::vector<Key> & vertices){
			
			unsigned int n = vertices.size();
			SplayTree<Key> * vv;

			for(unsigned int i = 0; i < n; ++i){
				vv = new SplayTree<Key>(vertices[i], vertices[i], ++id);
				this->mapNodes[vertices[i]][vertices[i]] = vv->root;
				this->mapTrees[vv->root->id] = vv;
			}
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*
		check whether nodes u and v are connected by
		comparing their splayTree ID's
		*/
		bool isConnected(Key u, Key v){

			unsigned int uSplayTree = find(mapNodes[u][u]);
			unsigned int vSplayTree = find(mapNodes[v][v]);

			if(uSplayTree == vSplayTree) return true;

			return false;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//create a link between nodes u and v
		void link(Key u, Key v){

			if(isConnected(u, v)) return;
			
			if(u > v) std::swap(u, v);
			
			nodeSet.insert({u, v});

			SplayTree<Key> * splayTree1 = mapTrees[find(mapNodes[u][u])];
			SplayTree<Key> * splayTree2 = mapTrees[find(mapNodes[v][v])];

			splayTree1 = bringToFront(splayTree1, u);
			splayTree2 = bringToFront(splayTree2, v);
			
			concatenate(splayTree1, u, splayTree2, v);			
		}
//+++++++++++++++a++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//cut the node with u and v ends
		void cut(Key u, Key v){

			if(!isConnected(u, v)) return;
			
			if(u > v) std::swap(u, v);
			
			nodeSet.erase({u, v});

			unsigned int uvPosition = order(mapNodes[u][v]);
			unsigned int vuPosition = order(mapNodes[v][u]);

			unsigned int uvID = find(mapNodes[u][v]);

			SplayTree<Key> * splayTree = mapTrees[uvID];

			if(uvPosition > vuPosition) std::swap(uvPosition, vuPosition);

			mapTrees.erase(uvID);
			
			std::pair<SplayTree<Key> *, SplayTree<Key> *> split1 = splayTree->split(uvPosition - 1);
			
			if(split1.first) vuPosition -= size(split1.first->root);

			std::pair<SplayTree<Key> *, SplayTree<Key> *> split2 = split1.second->split(1);
			
			std::pair<SplayTree<Key> *, SplayTree<Key> *> split3 = split2.second->split(vuPosition - 2);
			std::pair<SplayTree<Key> *, SplayTree<Key> *> split4 = split3.second->split(1);

			concatenate(split1.first, split4.second);

			if(split3.first != nullptr){
				mapTrees[split3.first->root->id] = split3.first;
			}

			mapNodes[u].erase(v);
			mapNodes[v].erase(u);

			delete(split2.first);
			delete(split4.first);
			split2.first = nullptr;
			split4.first = nullptr;		
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
		//print all the nodes from the forest
		void print(){
			
			auto node = nodeSet.begin();
			std::cout << node->first << " " << node->second;
			
			for (std::advance(node, 1); node != nodeSet.end(); ++node){
			  	std::cout << "  " << node->first << " " << node->second;
			}

			std::cout << '\n';
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//print all splay trees based on their ID
		void printTrees(){

			mapTrees.erase(0);
 			for(auto & splayTree : mapTrees){

				if(splayTree.second){
					std::cout << "Sequence ID: " << splayTree.first << "\n";
					splayTree.second->print();
				}	
			}
		}

		bool hasNode(Key u, Key v){
			
			if(mapNodes[u][v]) return true;
			return false;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		Node<Key> * getRoot(Key u){
			
			SplayTree<Key> * splayTree = mapTrees[find(mapNodes[u][u])];
			return splayTree->root;
		}
};
