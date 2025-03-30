#include <iostream>
#include <cmath>

template<typename Key>

class DynamicGraph{

	private:

		std::vector<AdjacencyList<Key> *> * adjacencyLists;
		
		std::vector<Forest<Key> *> * forests;

		std::unordered_map<Key, std::unordered_map<Key, unsigned int>>() mapLevels;
		
		unsigned int level;
		
	public:

		DynamicGraph(unsigned int n){
			
			level = static_cast<int>(std::ceil(std::log2(n))); 

			adjacencyLists = new std::vector<AdjacencyList<Key> *>(level + 1);
			forests = new std::vector<Forest<Key> *>(level + 1);
			
			for(int i = 0; i <= level; ++i){
				adjacencyLists[i] = new AdjacencyList<Key>();
				forests[i] = new Forest<Key>();
			}
		}

		void add(Key u, Key v){

			mapLevels[u][v] = level;

			AdjacencyList<Key> * adjList = this->adjacencyLists[level];
			Forest<Key> * forest = this->forests[level];

			if(forest->isConnected(u, v)) adjList->insert(u, v);
			else forest->add(u, v);
		}

		void remove(Key u, Key v){
		}

		bool isConnected(Key u, Key v){
			return true;
		}
}