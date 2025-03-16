template<typename Key>

class AdjacencyList{

	private:

		unordered_map<int, unordered_set<int>> adjList;

	public:

		void addEdge(Key u, Key v){
			
			this->adjList[u].insert(v);
			this->adjList[v].insert(u);
		}

		void removeEdge(Key u, Key v){
			
			this->adjList[u].erase(v);
        	this->adjList[v].erase(u);
		}
}