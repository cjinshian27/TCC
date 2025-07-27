template<typename Key>

class AdjacencyList{
    
	public: 
		std::unordered_map<Key, Node<Key> *> nodes;

		// adds egde (u, v) in O(lg(n)) time
		void add(Forest<Key> * forest, Key u, Key v, int weight){
			Node<Key> * nodeUU = forest->getNode(u, u);
			Node<Key> * nodeVV = forest->getNode(v, v);
			
			nodes[u] = nodeUU;
			nodes[v] = nodeVV;
			
			nodeUU->addNeighbor(nodeVV->first, weight);
            nodeVV->addNeighbor(nodeUU->first, weight);
		}

		// removes edge (u, v) in constant time O(1)
		void remove(Forest<Key> * forest, Key u, Key v){	
			Node<Key> * nodeUU = forest->getNode(u, u);
			Node<Key> * nodeVV = forest->getNode(v, v);
			
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