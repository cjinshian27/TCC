template<typename Key>

class AdjacencyList{
	
	public:
		std::unordered_map<Key, std::unordered_set<Key>> adjList;

		// add egde (u, v) in constant time O(1)
		void add(Key u, Key v){
			this->adjList[u].insert(v);
			this->adjList[v].insert(u);
		}

		// remove edge (u, v) in constant time O(1)
		void remove(Key u, Key v){
			this->adjList[u].erase(v);
        	this->adjList[v].erase(u);
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