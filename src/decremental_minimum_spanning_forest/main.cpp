#include <iostream>
#include <vector>
#include <algorithm>
#include "decrementalMSF.hpp"

/*
runs in O(mlg(m)) time, since we are sorting 
the edges in increasing weight
*/ 

DecrementalMSF<int> * buildDecrementalMSF(){
	
	unsigned int numberOfVertices;
	unsigned int numberOfEdges;
	unsigned int u, v, weight;
	
	std::cin >> numberOfVertices;
	std::vector<int> vertices(numberOfVertices);

	for(unsigned int i = 0; i < numberOfVertices; ++i){
		std::cin >> vertices[i];
	}

	std::cin >> numberOfEdges;
	std::vector<std::tuple<int, int, int>> edges(numberOfEdges);

	// sort the edges by increasing weight
    std::sort(edges.begin(), edges.end(), 
              [](const auto& a, const auto& b) {
                  return std::get<2>(a) < std::get<2>(b);
              });
	
	for(unsigned int i = 0; i < numberOfEdges; ++i){
		std::cin >> u >> v >> weight; 
		edges[i] = {u, v, weight};
	}
	
	DecrementalMSF<int> * decrementalMSF = new DecrementalMSF<int>(vertices);
	
	// add the edges beforehand (user is not allowed to add more later)
	for (const auto& [u, v, weight] : edges) {
		decrementalMSF->add(u, v, weight);
    }
	
	return decrementalMSF;
}

//decode function
void decode(){

	int operation, u, v;

	DecrementalMSF<int> * decrementalMSF = buildDecrementalMSF(); 

	while(std::cin >> operation){
		
		switch(operation){

			case 1:
				std::cin >> u >> v;
				if(decrementalMSF->areConnected(u, v)){
					std::cout << u << " and " << v << " are connected!" << '\n';
				} 
				else{
					std::cout << u << " and " << v << " are NOT connected!" << '\n';
				} 
				break;
			
			case 2: 
				std::cin >> u >> v;
				decrementalMSF->remove(u, v);
				break;

			case 3:
				decrementalMSF->print();
				break;
		}
	}

}

int main(){
	decode();
}