#include <iostream>
#include <cmath>

template<typename Key>

class DynamicGraph{

	private:

		std::vector<adjacencyList<Key> *> * adjacencyLists;
		std::vector<Forest<Key> *> * forests;

		unsigned int n;
	
	public:

		DynamicGraph(unsigned int n){
			adjacencyLists = new std::vector<adjacencyList<Key> *>(n + 1);
			forests = new std::vector<Forest<Key> *>(n + 1);
		}

		void add(Forest<Key> * forest, Key u, Key v){
			unsigned int level = static_cast<int>(std::ceil(std::log2(n))); 

			Edge<Key, Key> * edge = new Edge<Key, Key>(u, v);

			if(forest->isConnected(u, v)){
				this->adjacencyLists<Key>[level]->insert(u, v);
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