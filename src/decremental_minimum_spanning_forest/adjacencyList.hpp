#include <unordered_set>

template<typename Key>

class AdjacencyList{
    
	public: 
        std::unordered_map<int, std::unordered_set<int>> adjList;

		// adds egde (u, v) in O(lg(n)) time
		void add(Forest<Key> * forest, Key u, Key v, int weight){
			Node<Key> * nodeUU = forest->getNode(u, u);
			Node<Key> * nodeVV = forest->getNode(v, v);

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
		
		// print the adjancecy list from each node
		void print(){
			for (const auto& pair : adjList) {
				int vertex = pair.first;
				const std::unordered_set<int>& neighbors = pair.second;
				
				std::cout << vertex << ":";
				for (int neighbor : neighbors) {
					std::cout << " " << neighbor;
				}
				std::cout << std::endl;
			}
		}
};