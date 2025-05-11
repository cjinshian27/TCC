#include <vector>
#include "splayTree.hpp"

template<typename Key>

class DynamicForest{

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
		std::unordered_map<Key, SplayTree<Key> *> mapTrees; 

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

			SplayTree<Key> * aux = mapTrees[currentEdge->id];

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

			SplayTree<Key> * aux = mapTrees[currentEdge->id];
			
			if(aux){
				std::swap(currentEdge->id, edge->id);
				aux->splay(edge);
				mapTrees[edge->id] = aux;
			} 

			return size(edge->left) + 1;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//bring the edge that contains u to the front of the euler tour
		SplayTree<Key> * bringToFront(SplayTree<Key> * splayTree, Key u){
			
			if(!splayTree) return nullptr;
			
			unsigned int position = order(getEdge(u));
			
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

		/*
		get the first edge that contains u as the first end.
			
		example: (u, v), (u, w), and (u, x)
		*/
		Edge<Key> * getEdge(Key u){
			
			auto mapValue = mapEdges[u].begin();

			if(mapValue != mapEdges[u].end()){
				return mapValue->second;
			}

			return nullptr;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//concatenate tree1, uv, tree2, and vu
		void concatenate(SplayTree<Key> * tree1, Key u, SplayTree<Key> * tree2, Key v){
			
			SplayTree<Key> * uv = new SplayTree<Key>(u, v, ++id);
			SplayTree<Key> * vu = new SplayTree<Key>(v, u, ++id);

			this->mapEdges[u][v] = uv->root;
			this->mapEdges[v][u] = vu->root;

			if(tree1){

				tree1->join(uv);
				tree1->join(tree2);
				tree1->join(vu);

				mapTrees[tree1->root->id] = tree1;
			}
			else{

				uv->join(tree2);
				uv->join(vu);

				mapTrees[uv->root->id] = uv;
			}
		}	

	public:	

		DynamicForest(std::vector<Key> & vertices){
			
			unsigned int n = vertices.size();
			Key v;
			SplayTree<Key> * vv;

			for(unsigned int i = 0, i < n; ++i){
				v = vertices[i];
				vv = new SplayTree<Key>(v, v, ++id);
				this->mapEdges[v][v] = vv->root;
			}

		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*
		check whether nodes u and v are connected. 
		
		in the if statement(), the additional getEdge() 
		checking is for the case when id1 and id2 return 
		0. That means there are no edges with ends u or 
		v, so it will return a nullptr. Since a nullptr
		indicates false, then it will not return true.
		*/
		bool isConnected(Key u, Key v){

			unsigned int id1 = find(mapEdges[u][u]);
			unsigned int id2 = find(mapEdges[v][v]);

			if(id1 == id2 && mapEdges[u][u]) return true;

			return false;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//create a link between nodes u and v
		void link(Key u, Key v){

			if(isConnected(u, v)) return;
			
			if(u > v) std::swap(u, v);
			
			edgeSet.insert({u, v});

			SplayTree<Key> * splayTree1 = mapTrees[find(mapEdges[u][u])];
			SplayTree<Key> * splayTree2 = mapTrees[find(mapEdges[v][v])];

			splayTree1 = bringToFront(splayTree1, u);
			splayTree2 = bringToFront(splayTree2, v);

			concatenate(splayTree1, u, splayTree2, v);			
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
 			for(auto & splayTree : mapTrees){

				if(splayTree.second){
					std::cout << "Sequence ID: " << splayTree.first << "\n";
					splayTree.second->print();
				}	
			}
		}

		bool hasEdge(Key u, Key v){
			
			if(mapEdges[u][v]) return true;
			return false;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		Edge<Key> * getRoot(Key u){
			
			SplayTree<Key> * splayTree = mapTrees[find(mapEdges[u][u])];
			return splayTree->root;
		}
};
