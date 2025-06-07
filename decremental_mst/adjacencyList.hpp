template<typename Key>

class AdjacencyList{
	
	private:
		// Store edges in a doubly-linked list to maintain order
		std::unordered_map<int, std::list<std::pair<int, int>>> adjList; // vertex -> list of (neighbor, weight)
		
		// Store iterators for O(1) lookup and removal
		std::unordered_map<int, std::unordered_map<int, std::list<std::pair<int, int>>::iterator>> edgeIters;

	public:
		// Add edge in O(1) time
		void addEdge(int u, int v, int w) {
			// Add edge (u,v)
			adjList[u].push_back({v, w});
			auto it = std::prev(adjList[u].end()); // Iterator to the new element
			edgeIters[u][v] = it;
			
			// For undirected graph, add (v,u) as well
			adjList[v].push_back({u, w});
			it = std::prev(adjList[v].end());
			edgeIters[v][u] = it;
		}
		
		// Remove edge in O(1) time while preserving order
		void removeEdge(int u, int v) {
			if (edgeIters[u].count(v) == 0) return; // Edge doesn't exist
			
			// Remove edge (u,v) using stored iterator - O(1) operation
			adjList[u].erase(edgeIters[u][v]);
			edgeIters[u].erase(v);
			
			// Remove reverse edge (v,u)
			adjList[v].erase(edgeIters[v][u]);
			edgeIters[v].erase(u);
		}

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