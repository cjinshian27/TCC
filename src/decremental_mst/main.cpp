#include <iostream>
#include <utility>
#include <unordered_map>
#include <vector>
#include <string>
#include <iterator>
#include <tuple>
#include <set>
#include <unordered_set>
#include <cmath>
#include "decrementalMST.hpp"

DecrementalMST<int> * buildDecrementalMST(){
	
	unsigned int numberOfVertices;
	unsigned int numberOfEdges;
	unsigned int u, v, weight;
	
	std::cin >> numberOfVertices;
	std::cin >> numberOfEdges;
	std::vector<int> edges(numberOfEdges);
	
	for(unsigned int i = 0; i < numberOfEdges; ++i){
		std::cin >> u >> v >> weight; 
		edges.push_back(std::make_tuple(u, v, weight));  // Edge from 0 to 1 with weight 5
	}
	
	return new DecrementalMST<int>(numberOfVertices, edges);
}

//decode function
void decode(){

	int operation, u, v, weight;

	DecrementalMST<int> *  = buildDynamicGraph(); 

	while(std::cin >> operation){
		
		switch(operation){

			case 1:
				std::cin >> u >> v;
				if(decrementalMST->isConnected(u, v)){
					std::cout << u << " and " << v << " are connected!" << '\n';
				} 
				else{
					std::cout << u << " and " << v << " are NOT connected!" << '\n';
				} 
				break;
			
			case 2: 
				std::cin >> u >> v;
				decrementalMST->remove(u, v);
				break;

			case 3:
				decrementalMST->print();
				break;
		}
	}

}

int main(){
	decode();
}