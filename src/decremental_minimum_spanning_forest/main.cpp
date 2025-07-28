#include <iostream>
#include <vector>
#include "decrementalMSF.hpp"

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
	DecrementalMSF<int> * decrementalMSF = new DecrementalMSF<int>(vertices);
	
	for(unsigned int i = 0; i < numberOfEdges; ++i){
		std::cin >> u >> v >> weight; 
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
				if(decrementalMSF->isConnected(u, v)){
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