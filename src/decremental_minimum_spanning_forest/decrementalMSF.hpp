#pragma once

#include "./helpful_classes/node.hpp"
#include "./helpful_classes/tree.hpp"
#include "./helpful_classes/forest.hpp"
#include "./helpful_classes/adjacencyList.hpp"

#include <cmath>

inline void printStylishLine(){
	std::cout << "════════════════════════════════════════════════════════════════════════════════════\n";
}
template<typename Key>

class DecrementalMSF{
	
	private:

	// store the adjacency lists (each one stores backup nodes)
	std::vector<AdjacencyList<Key> *> adjacencyLists;
	
	// store the forests 
	std::vector<Forest<Key> *> forests;
	
	// map the this->maxLevel of the nodes 
	std::unordered_map<Key, std::unordered_map<Key, unsigned int>> mapNodeLevels;
	
	unsigned int maxLevel;
	
	// update node forest level
	void updateMapNodeLevels(Key u, Key v, unsigned int level){
		this->mapNodeLevels[u][v] = level;
		this->mapNodeLevels[v][u] = level;
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	// decrease the node forest level
	void decreaseNodesLevel(Tree<Key> * treeContainingU, unsigned int i){
		
		Node<Key> * nodeToSplay = treeContainingU->getNodeWithIsLevelTrue(treeContainingU->root);
		
		this->forests[i]->splayNode(treeContainingU, nodeToSplay);

		treeContainingU->root->isLevel = 0;
		treeContainingU->root->setNodeLevelCount();
		
		Key u = treeContainingU->root->first;
		Key v = treeContainingU->root->second;
		updateMapNodeLevels(u, v, i - 1);
		
		this->forests[i - 1]->link(u, v, nodeToSplay->weight);
	}	
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	/*
	after an node (edge) is cut, find a replacement node (edge) 
	in the adjacency list to link the two split forests again
	*/
	void replaceNode(Key u, Key v, unsigned int nodeLevel){
		
		bool nodeIsReplaced = false;

		for(unsigned int i = nodeLevel; i <= this->maxLevel && !nodeIsReplaced; ++i){
			
			Tree<Key> * treeContainingU = this->forests[i]->getTreeContaining(u);
			Tree<Key> * treeContainingV = this->forests[i]->getTreeContaining(v);
			
			if(treeContainingU->size() > treeContainingV->size()){
				std::swap(treeContainingU, treeContainingV);
			} 
			
			/*
			the tree that contains u has less than 2^i nodes now, 
			1 ≤ i ≤ ⌈lg(n)⌉, so we need to decrease the node levels by 1
			*/ 
			while(treeContainingU->root->nodesAtLevel > 0){
				decreaseNodesLevel(treeContainingU, i);
			}
			
			/*
			since treeContainingU has fewer nodes than treeContaningV, 
			then we need to find a replacement node (edge) that has one 
			end in treeContaningU and another end in treeContaningV. 
			
			This replacement node is called reserve node, and the nodes
			that are not reserve are removed later on. 
			*/
			while(treeContainingU->root->reserveNodes > 0 && !nodeIsReplaced){
				
				Node<Key> * nodeXX = treeContainingU->getNodeWithTheLightestIncidentEdge(treeContainingU->root);
				forests[i]->splayNode(treeContainingU, nodeXX);

				std::pair<Key, int> neighbor = nodeXX->neighbors->extractMin();
				
				Key x = nodeXX->first;
				Key y = neighbor.first;
				int nodeXYWeight = neighbor.second;

				std::cout << "<" << x << ":"  << y << ">" << std::endl;
			
				/*
				if the nodes x and y are connected, then we know 
				they are in treeContaningU, because they are in 
				the same component. So we just decrease the level 
				of (x, y). Otherwise, we found a replacement node, 
				and we link x and y on every forest from the current
				level to ⌈lg(n)⌉. 
				*/
				if(this->forests[i]->isConnected(x, y)){
					updateMapNodeLevels(x, y, i - 1);
					Node<Key> * nodeXX = this->forests[i - 1]->getNode(x, x);
					Node<Key> * nodeYY = this->forests[i - 1]->getNode(y, y);

					this->adjacencyLists[i - 1]->add(nodeXX, nodeYY, nodeXYWeight);
					this->forests[i - 1]->increaseIncidentToReserveNodeCount(nodeXX);
					this->forests[i - 1]->increaseIncidentToReserveNodeCount(nodeYY);
				}
				else{													
					for(unsigned int j = i; j <= this->maxLevel; ++j){
						this->forests[j]->link(x, y, nodeXYWeight);
					}
					nodeIsReplaced = true;
				}
		
				/*
				remove nodes that are incident to treeContainingU, but not 
				to treeContainingV 
				*/
				Node<Key> * nodeYY = this->forests[i]->getNode(y, y);

				this->adjacencyLists[i]->remove(nodeXX, nodeYY);
				this->forests[i]->decreaseIncidentToReserveNodeCount(nodeYY);

				/*
				if x is not incident to any other node of level i, then
				decrease its reserve node count
				*/
				if(nodeXX->neighbors->isEmpty()){
					this->forests[i]->decreaseIncidentToReserveNodeCount(nodeXX);
				}
			}
		}
	}

	public:

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// instantiate a dynamic graph in O(lg(n)) time.
		DecrementalMSF(std::vector<Key> & vertices){
			
			this->maxLevel = static_cast<int>(std::ceil(std::log2(vertices.size()))); 

			this->adjacencyLists = std::vector<AdjacencyList<Key> *>(this->maxLevel + 1);
			this->forests = std::vector<Forest<Key> *>(this->maxLevel + 1);
			
			for(unsigned int i = 0; i <= this->maxLevel; ++i){
				adjacencyLists[i] = new AdjacencyList<Key>();
				forests[i] = new Forest<Key>(vertices);
			}
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// add node <u, v, weight> in O(lg(n))
		void add(Key u, Key v, int weight){

			updateMapNodeLevels(u, v, this->maxLevel);
			Forest<Key> * maxLevelForest = this->forests[this->maxLevel];

			if(maxLevelForest->isConnected(u, v)){
				Node<Key> * nodeUU = maxLevelForest->getNode(u,u);
				Node<Key> * nodeVV = maxLevelForest->getNode(v,v);

				this->adjacencyLists[this->maxLevel]->add(nodeUU, nodeVV, weight);
				maxLevelForest->increaseIncidentToReserveNodeCount(nodeUU);
				maxLevelForest->increaseIncidentToReserveNodeCount(nodeVV);
			} 
			else{
				maxLevelForest->link(u, v, weight);
			} 
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				
		//remove the node (edge) <u, v, weight> in O(lg²(n))
		void remove(Key u, Key v){
			
			unsigned int nodeLevel = mapNodeLevels[u][v];
			mapNodeLevels[u].erase(v);
			mapNodeLevels[v].erase(u);
			
			/*
			if the forest of level ⌈lg(n)⌉ has <u, v, weight>, 
			then we need to find a replacement for <u, v, weight>
			so the forest is not disconnected. Otherwise, 
			<u, v, weight> is a reserve node and the forest 
			is still maintained, so we simply need to remove 
			<u, v> from the adjacency list
			*/

			Forest<Key> * maxLevelForest = this->forests[this->maxLevel];
			if(maxLevelForest->hasNode(u, v)){
				for(unsigned int i = nodeLevel; i <= this->maxLevel; ++i)
					 this->forests[i]->cut(u, v);

				replaceNode(u, v, nodeLevel);
			}
			else{
				Forest<Key> * nodeLevelForest = this->forests[nodeLevel];
				Node<Key> * nodeUU = nodeLevelForest->getNode(u, u);
				Node<Key> * nodeVV = nodeLevelForest->getNode(v, v);

				this->adjacencyLists[nodeLevel]->remove(nodeUU, nodeVV);
				nodeLevelForest->decreaseIncidentToReserveNodeCount(nodeUU);
				nodeLevelForest->decreaseIncidentToReserveNodeCount(nodeVV);
			}
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// checks if u and v are connected in O(lg(n))
		bool isConnected(Key u, Key v){
			
			Forest<Key> * forest = this->forests[this->maxLevel];
			return forest->isConnected(u, v);
		}

		// prints out all the dynamic graph in O(nlg(n))
		void print(){
			printStylishLine();
			std::cout << "FORESTS: \n\n";
			for(int i = this->maxLevel; i >= 0; --i){
				std::cout << "FOREST LEVEL " << i + 1 << ":\n";
				this->forests[i]->printTrees();
				std::cout << "**************************************************\n";
			}

			std::cout << "\n\n\n\n"; 

			std::cout << "ADJACENCY LISTS \n\n";
			for(int i = this->maxLevel; i >= 0; --i){
				std::cout << "ADJACENCY LIST LEVEL " << i + 1 << ":\n";
				this->adjacencyLists[i]->print();
				std::cout << "**************************************************\n";
			}
			printStylishLine();
		}
};