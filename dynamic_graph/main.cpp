#include <iostream>
#include <utility>
#include <unordered_map>
#include <vector>
#include <string>
#include <iterator>
#include <set>
#include <unordered_set>
#include <cmath>
#include "dynamicGraph.hpp"

DynamicGraph<int> * buildDynamicGraph(){
	
	unsigned int n;
	
	std::cin >> n;
	std::vector<int> vertices(n);
	
	for(unsigned int i = 0; i < n; ++i){
		std::cin >> vertices[i];
	}
	return new DynamicGraph<int>(vertices);
}

//decode function
void decode(){

	int operation, u, v;

	DynamicGraph<int> * dynamicGraph = buildDynamicGraph(); 

	while(std::cin >> operation){
		
		switch(operation){

			case 1:
				std::cin >> u >> v;
				if(dynamicGraph->isConnected(u, v)){
					std::cout << u << " and " << v << " are connected!" << '\n';
				} 
				else{
					std::cout << u << " and " << v << " are NOT connected!" << '\n';
				} 
				break;
		
			case 2:
				std::cin >> u >> v;
				dynamicGraph->add(u, v);
				break;
			
			case 3:
				std::cin >> u >> v;
				dynamicGraph->remove(u, v);
				break;

			case 4:
				dynamicGraph->print();
				break;
		}
	}

}

int main(){
	decode();
}