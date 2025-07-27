#pragma once

#include <set>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "node.hpp"
#include "tree.hpp"


template<typename Key>

class Forest{

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
		std::unordered_map<Key, Tree<Key> *> mapTrees; 

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

			Tree<Key> * aux = mapTrees[currentNode->id];

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

			Tree<Key> * aux = mapTrees[currentNode->id];
			
			if(aux){
				std::swap(currentNode->id, node->id);
				aux->splay(node);
				mapTrees[node->id] = aux;
			} 

			return size(node->left) + 1;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//bring the node that contains u to the front of the euler tour
		Tree<Key> * bringToFront(Tree<Key> * tree, Key u){
			
			if(!tree) return nullptr;
			
			unsigned int position = order(mapNodes[u][u]);
			
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
			uv->root->isLevel = true;

			this->mapNodes[u][v] = uv->root;
			this->mapNodes[v][u] = vu->root;

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
				this->mapNodes[vertices[i]][vertices[i]] = vv->root;
				this->mapTrees[vv->root->id] = vv;
			}
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*
		check whether nodes u and v are connected by
		comparing their tree ID's
		*/
		bool isConnected(Key u, Key v){

			unsigned int uTreeID = find(mapNodes[u][u]);
			unsigned int vTreeID = find(mapNodes[v][v]);

			if(uTreeID == vTreeID){

				return true;
			} 

			return false;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//create a link between nodes u and v
		void link(Key u, Key v){

			if(isConnected(u, v)) return;
			
			if(u > v) std::swap(u, v);
			
			nodeSet.insert({u, v});

			Tree<Key> * tree1 = mapTrees[find(mapNodes[u][u])];
			Tree<Key> * tree2 = mapTrees[find(mapNodes[v][v])];

			tree1 = bringToFront(tree1, u);
			tree2 = bringToFront(tree2, v);
			
			concatenate(tree1, u, tree2, v);			
		}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//cut the node with u and v ends
		void cut(Key u, Key v){

			if(!isConnected(u, v)) return;
			
			if(u > v) std::swap(u, v);
			
			nodeSet.erase({u, v});

			unsigned int uvPosition = order(mapNodes[u][v]);
			unsigned int vuPosition = order(mapNodes[v][u]);

			unsigned int uvID = find(mapNodes[u][v]);

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

			if(nodeSet.empty()){
				std::cout << "NO EDGES WERE CREATED YET!\n";
				return;
			}
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
 			for(auto & tree : mapTrees){

				if(tree.second){
					std::cout << "Sequence ID: " << tree.first << "\n";
					tree.second->print();
				}	
			}
		}

		bool hasNode(Key u, Key v){
			
			if(mapNodes[u][v]) return true;
			return false;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		Tree<Key> * getTreeContaining(Key u){
			
			Tree<Key> * tree = mapTrees[find(mapNodes[u][u])];
			return tree;
		}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// decrease the reserveNodes count
		void decreaseIncidentToReserveNodeCount(Key u){
			
			Tree<Key> * tree = mapTrees[find(mapNodes[u][u])];
			tree->splay(mapNodes[u][u]);
			mapNodes[u][u]->isIncidentToReserveNode = false;
			mapNodes[u][u]->setReserveNodesCount();
		}
		
		// increase the reserveNodes count
		void increaseIncidentToReserveNodeCount(Key u){
			
			Tree<Key> * tree = mapTrees[find(mapNodes[u][u])];
			tree->splay(mapNodes[u][u]);
			mapNodes[u][u]->isIncidentToReserveNode = true;
			mapNodes[u][u]->setReserveNodesCount();
		}
		
		// return the node uu
		Node<Key> * getNode(Key u){
			return this->mapNodes[u][u];
		}
};
