template<typename Key>

class AdjacencyList{

	private:

		unordered_map<int, unordered_set<int>> adjList;

	public:

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
};