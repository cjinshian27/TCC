#include "tree.hpp"

template<typename Key>

class Forest{

	private:
		unsigned int id = 0;

		/*
		keep the remaining edges in the forest in 
		a <Key, Key> format.
		*/
		std::set<std::pair<Key, Key>> edgeSet;

		/*
		map the edges according to their two (Key) ends. 
		That means mapEdges[u][v] = Edge<Key>(u, v).
		*/
		std::unordered_map<Key, std::unordered_map<Key, Edge<Key> *>> mapEdges; 
		
		/*
		map the remaining splay trees in the forest, where 
		an id is used to identify each splay tree uniquely
		*/
		std::unordered_map<Key, Tree<Key> *> mapTrees; 

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		/*
		get the size of an edge, which means the sum 
		of its left subtree, right subtree + 1. 
		*/
		unsigned int size(Edge<Key> * edge){
			
			if(!edge) return 0;
			return edge->size;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		/*
		return the id of the root whose tree contains edge
		
		example: splaying [2:1] node
			
		([1:2] id1)      (swap ids)   ([1:2] id2)     (splay)    ([2:1] id1)        
		      \             ▶▶▶            \            ▶▶▶        /
		  	 ([2:1] id2)                  ([2:1] id1)         ([1:2] id2)
											
		*/
		unsigned int find(Edge<Key> * edge){	
			
			if(!edge) return 0;	
			
			Edge<Key> * currentEdge = edge;

			while(currentEdge->parent){
				currentEdge = currentEdge->parent;
			}

			Tree<Key> * aux = mapTrees[currentEdge->id];

			if(aux){
				std::swap(currentEdge->id, edge->id);
				aux->splay(edge);
				mapTrees[edge->id] = aux;
			} 
			
			return edge->id;
		}   

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		/*
		return the rank/order of an edge in the tree
		
		example: splaying [2:1] node
			
		([1:2] id1)      (swap ids)   ([1:2] id2)     (splay)    ([2:1] id1)        
		      \             ▶▶▶            \            ▶▶▶        /
		  	 ([2:1] id2)                  ([2:1] id1)         ([1:2] id2)
		*/
		unsigned int order(Edge<Key> * edge){
			
			Edge<Key> * currentEdge = edge;

			while(currentEdge->parent) currentEdge = currentEdge->parent;

			Tree<Key> * aux = mapTrees[currentEdge->id];
			
			if(aux){
				std::swap(currentEdge->id, edge->id);
				aux->splay(edge);
				mapTrees[edge->id] = aux;
			} 

			return size(edge->left) + 1;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//bring the edge that contains u to the front of the euler tour
		Tree<Key> * bringToFront(Tree<Key> * tree, Key u){
			
			if(!tree) return nullptr;
			
			unsigned int position = order(mapEdges[u][u]);
			
			mapTrees.erase(tree->root->id);

			std::pair<Tree<Key> *, Tree<Key> *> pairOfTrees = tree->split(position - 1);
			
			Tree<Key> * aux = pairOfTrees.second;
			aux->join(pairOfTrees.first);

			return aux;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//concatenate two splay trees
		void concatenate(Tree<Key> * tree1, Tree<Key> * tree2){
			
			if(!tree1){
				mapTrees[tree2->root->id] = tree2;
				return;
			}
			
			tree1->join(tree2);
			mapTrees[tree1->root->id] = tree1;
		}
		
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
		//concatenate four pieces to form the forest: tree1, uv, tree2, vu
		void concatenate(Tree<Key> * tree1, Key u, Tree<Key> * tree2, Key v){
			
			Tree<Key> * uv = new Tree<Key>(u, v, ++id);
			Tree<Key> * vu = new Tree<Key>(v, u, ++id);
			
			this->mapEdges[u][v] = uv->root;
			this->mapEdges[v][u] = vu->root;

			tree1->join(uv);
			tree1->join(tree2);
			tree1->join(vu);
			mapTrees[tree1->root->id] = tree1;
		}	
		
	public:	

		Forest(std::vector<Key> & vertices){
			
			unsigned int n = vertices.size();
			Tree<Key> * vv;

			for(unsigned int i = 0; i < n; ++i){
				vv = new Tree<Key>(vertices[i], vertices[i], ++id);
				this->mapEdges[vertices[i]][vertices[i]] = vv->root;
				this->mapTrees[vv->root->id] = vv;
			}
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*
		check whether nodes u and v are connected by
		comparing their tree ID's
		*/
		bool isConnected(Key u, Key v){

			unsigned int uTree = find(mapEdges[u][u]);
			unsigned int vTree = find(mapEdges[v][v]);

			if(uTree == vTree) return true;

			return false;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//create a link between nodes u and v
		void link(Key u, Key v){

			if(isConnected(u, v)) return;
			
			if(u > v) std::swap(u, v);
			
			edgeSet.insert({u, v});

			Tree<Key> * tree1 = mapTrees[find(mapEdges[u][u])];
			Tree<Key> * tree2 = mapTrees[find(mapEdges[v][v])];

			tree1 = bringToFront(tree1, u);
			tree2 = bringToFront(tree2, v);
			
			concatenate(tree1, u, tree2, v);			
		}
//+++++++++++++++a++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//cut the edge with u and v ends
		void cut(Key u, Key v){

			if(!isConnected(u, v)) return;
			
			if(u > v) std::swap(u, v);
			
			edgeSet.erase({u, v});

			unsigned int uvPosition = order(mapEdges[u][v]);
			unsigned int vuPosition = order(mapEdges[v][u]);

			unsigned int uvID = find(mapEdges[u][v]);

			Tree<Key> * tree = mapTrees[uvID];

			if(uvPosition > vuPosition) std::swap(uvPosition, vuPosition);

			mapTrees.erase(uvID);
			
			std::pair<Tree<Key> *, Tree<Key> *> split1 = tree->split(uvPosition - 1);
			
			if(split1.first) vuPosition -= size(split1.first->root);

			std::pair<Tree<Key> *, Tree<Key> *> split2 = split1.second->split(1);
			
			std::pair<Tree<Key> *, Tree<Key> *> split3 = split2.second->split(vuPosition - 2);
			std::pair<Tree<Key> *, Tree<Key> *> split4 = split3.second->split(1);

			concatenate(split1.first, split4.second);

			if(split3.first != nullptr){
				mapTrees[split3.first->root->id] = split3.first;
			}

			mapEdges[u].erase(v);
			mapEdges[v].erase(u);

			delete(split2.first);
			delete(split4.first);
			split2.first = nullptr;
			split4.first = nullptr;		
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
		//print all the edges from the forest
		void print(){
			
			auto edge = edgeSet.begin();
			std::cout << edge->first << " " << edge->second;
			
			for (std::advance(edge, 1); edge != edgeSet.end(); ++edge){
			  	std::cout << "  " << edge->first << " " << edge->second;
			}

			std::cout << '\n';
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//print all splay trees based on their ID
		void printTrees(){

			mapTrees.erase(0);
 			for(auto & tree : mapTrees){

				if(tree.second){
					std::cout << "Sequence ID: " << tree.first << "\n";
					tree.second->print();
				}	
			}
		}

		bool hasEdge(Key u, Key v){
			
			if(mapEdges[u][v]) return true;
			return false;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		Tree<Key> * getTreeContaining(Key u){
			
			Tree<Key> * tree = mapTrees[find(mapEdges[u][u])];
			return tree;
		}
};
