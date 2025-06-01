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
	
	unsigned int n;
	
	std::cin >> n;
	std::vector<int> vertices(n);
	
	for(unsigned int i = 0; i < n; ++i){
		std::cin >> vertices[i];
	}
	return new DecrementalMST<int>(vertices);
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
				std::cin >> u >> v >> weight;
				decrementalMST->add(u, v, weight);
				break;
			
			case 3: 
				std::cin >> u >> v;
				decrementalMST->remove(u, v);
				break;

			case 4:
				decrementalMST->print();
				break;
		}
	}

}

int main(){
	decode();
}