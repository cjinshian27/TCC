
#include "kruskal.hpp"
#include <iostream>
#include <chrono>

Graph buildGraph() {
    
    unsigned int numberOfVertices;
	unsigned int numberOfEdges;
	unsigned int u, v, weight;
	
	std::cin >> numberOfVertices;
 
    Graph g(numberOfVertices);
    
    std::cin >> numberOfEdges;

    // add edges (u, v, weight)
	for(unsigned int i = 0; i < numberOfEdges; ++i){
        std::cin >> u >> v >> weight;
        g.addEdge(u, v, weight);
    }
        
    g.buildMST();
    return g;
}

void decode(){

	int operation, u, v;

	Graph graph = buildGraph(); 

	int totalRemoved = 0;
	int batch = 25;
	auto start = std::chrono::steady_clock::now();

	while(std::cin >> operation){
		
		if(totalRemoved > 12800) break;

		if(totalRemoved == batch){
			auto end = std::chrono::steady_clock::now();
    		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

			std::cout << "Elapsed time: " << elapsed.count() << " milliseconds for batch " << batch << " - MST weight: " <<  graph.getTotalWeight() << std::endl;
		
			batch = batch + batch;
		}

		switch(operation){

			case 1:
				std::cin >> u >> v;
				if(graph.areConnected(u, v)){
					std::cout << u << " and " << v << " are connected!" << '\n';
				} 
				else{
					std::cout << u << " and " << v << " are NOT connected!" << '\n';
				} 
				break;
			
			case 2: 
				std::cin >> u >> v;
				++totalRemoved;
				graph.removeEdge(u, v);
				break;

			case 3:
				std::cout << graph.getTotalWeight() << std::endl;
				break;
			
		}
	}

}

int main(){
    decode();
    return 0;
}