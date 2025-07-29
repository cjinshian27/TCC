#pragma once

#include "node.hpp"
#include "splayTree.hpp"

#include <utility>
#include <iostream>
#include <unordered_map>
#include <set>

template<typename Key>

class DynamicForest{

	private:
		unsigned int id = 0;
		unsigned int numberOfNodes;

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
			
			unsigned int position = order(getNode(u));
			
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
		get the first node that contains u as the first end.
			
		example: (u, v), (u, w), and (u, x)
		*/
		Node<Key> * getNode(Key u){
			
			auto mapValue = mapNodes[u].begin();

			if(mapValue != mapNodes[u].end()){
				return mapValue->second;
			}

			return nullptr;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//concatenate tree1, uv, tree2, and vu
		void concatenate(SplayTree<Key> * tree1, Key u, SplayTree<Key> * tree2, Key v){
			
			SplayTree<Key> * uv = new SplayTree<Key>(u, v, ++id);
			SplayTree<Key> * vu = new SplayTree<Key>(v, u, ++id);

			this->mapNodes[u][v] = uv->root;
			this->mapNodes[v][u] = vu->root;

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

		DynamicForest(unsigned int n){
			
			this->numberOfNodes = n;
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*
		check whether nodes u and v are connected. 
		
		in the if statement(), the additional getNode() 
		checking is for the case when id1 and id2 return 
		0. That means there are no nodes with ends u or 
		v, so it will return a nullptr. Since a nullptr
		indicates false, then it will not return true.
		*/
		bool areConnected(Key u, Key v){

			unsigned int id1 = find(getNode(u));
			unsigned int id2 = find(getNode(v));

			if(id1 == id2 && getNode(u)) return true;

			return false;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//create a link between nodes u and v
		void link(Key u, Key v){

			if(areConnected(u, v)) return;
			
			if(u > v) std::swap(u, v);
			
			nodeSet.insert({u, v});

			SplayTree<Key> * splayTree1 = mapTrees[find(getNode(u))];
			SplayTree<Key> * splayTree2 = mapTrees[find(getNode(v))];

			splayTree1 = bringToFront(splayTree1, u);
			splayTree2 = bringToFront(splayTree2, v);

			concatenate(splayTree1, u, splayTree2, v);			
		}
//+++++++++++++++a++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//cut the node with u and v ends
		void cut(Key u, Key v){

			if(!areConnected(u, v)) return;
			
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
			
			SplayTree<Key> * splayTree = mapTrees[find(getNode(u))];
			return splayTree->root;
		}
};
