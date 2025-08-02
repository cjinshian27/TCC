
#include "kruskal.hpp"
#include <iostream>

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

	while(std::cin >> operation){
		
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
				graph.removeEdge(u, v);
				break;

			case 3:
				graph.printMST();
				break;
		}
	}

}

int main(){
    decode();
    return 0;
}