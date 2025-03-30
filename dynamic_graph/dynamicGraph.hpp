#include <iostream>
#include <cmath>

template<typename Key>

class DynamicGraph{

	private:

		std::vector<AdjacencyList<Key> *> * adjacencyLists;
		
		std::vector<Forest<Key> *> * forests;

		unsigned int n;
	
	public:

		DynamicGraph(unsigned int n){
			
			adjacencyLists = new std::vector<AdjacencyList<Key> *>(n + 1);
			forests = new std::vector<Forest<Key> *>(n + 1);
			
			for(int i = 0; i < level + 1; ++i){
				adjacencyLists[i] = new AdjacencyList<Key>();
				forests[i] = new Forest<Key>();
			}
		}

		void add(Forest<Key> * forest, Key u, Key v){
			unsigned int level = static_cast<int>(std::ceil(std::log2(n))); 

			Edge<Key, Key> * edge = new Edge<Key, Key>(u, v);
			
			AdjacencyList<Key> * adjList = this->adjacencyLists[level];
			Forest<Key> * forest = this->forests[level];

			if(forest->isConnected(u, v)){
				adjList->insert(u, v);
			}
			else{
				
			}

		}

		void remove(Key u, Key v){

		}

		bool isConnected(Key u, Key v){
			return true;
		}
}