#include <iostream>
#include <utility>
#include <unordered_map>
#include <vector>
#include <string>
#include <iterator>
#include <set>
#include <unordered_set>
#include <cmath>
#include "decrementalMST.hpp"

DecrementalMST<int> * buildDecrementalMST(){
	
	unsigned int numberOfVertices;
	unsigned int numberOfEdges;
	
	std::cin >> numberOfVertices;
	std::vector<int> vertices(numberOfVertices);
	
	for(unsigned int i = 0; i < numberOfVertices; ++i){
		std::cin >> vertices[i];
	}

	std::cin >> numberOfEdges;
	
	DecrementalMST<int> * decrementalMST = new DecrementalMST<int>(vertices);

	unsigned int u, v, weight;

	while(numberOfEdges--){
		std::cin >> u >> v >> weight;
		decrementalMST->link(u, v, weight);
	}
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