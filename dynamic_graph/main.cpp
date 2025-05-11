#include <iostream>
#include <utility>
#include <unordered_map>
#include <vector>
#include <string>
#include <iterator>
#include <set>
#include "dynamicGraph.hpp"

DynamicForest<int> * buildDynamicGraph(){
	
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

	DynamicGraph<int> * dynamicGraph = DynamicGraph(); 

	while(std::cin >> operation){
		
		switch(operation){

            // to do
		}
	}

}

int main(){
	decode();
}