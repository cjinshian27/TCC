#pragma once
#include "node.hpp"

template<typename Key>

class AdjacencyList{
    
	public: 
		std::unordered_map<Key, Node<Key> *> nodes;

		// adds egde (u, v) in O(lg(n)) time
		void add(Node<Key> * nodeUU, Node<Key> * nodeVV, int weight){
			nodes[nodeUU->first] = nodeUU;
			nodes[nodeVV->first] = nodeVV;
			
			nodeUU->addNeighbor(nodeVV->first, weight);
            nodeVV->addNeighbor(nodeUU->first, weight);
		}

		// removes edge (u, v) in constant time O(lg(n))
		void remove(Node<Key> * nodeUU, Node<Key> * nodeVV){
			nodeUU->removeNeighbor(nodeVV->first);
            nodeVV->removeNeighbor(nodeUU->first);
		}
		
		// print the adjacency list from each node
		void print(){
			for (const auto& pair : nodes) {
				Key key = pair.first;
				Node<Key> * node = pair.second;
				
				std::cout << "Key: " << key;
				node->neighbors->print();
			}
		}
};