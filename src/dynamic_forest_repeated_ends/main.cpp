#include <iostream>
#include <utility>
#include <unordered_map>
#include <vector>
#include <string>
#include <iterator>
#include <set>
#include "dynamicForest.hpp"


DynamicForest<int> * buildDynamicForest(){
	
	unsigned int n;
	
	std::cin >> n;
	std::vector<int> vertices(n);
	
	for(unsigned int i = 0; i < n; ++i){
		std::cin >> vertices[i];
	}
	return new DynamicForest<int>(vertices);
}

//decode function
void decode(){

	int operation, u, v;

	DynamicForest<int> * dynamicForest = buildDynamicForest(); 

	while(std::cin >> operation){
		
		switch(operation){
			
			case 1:
				std::cin >> u >> v;
				if(dynamicForest->areConnected(u, v)) std::cout << "True" << '\n';
				else std::cout << "False" << '\n';
				break;
			
			case 2:
				std::cin >> u >> v;
				dynamicForest->link(u, v);
				break;
			
			case 3:
				std::cin >> u >> v;
				dynamicForest->cut(u, v);
				break;
			
			case 4:
				dynamicForest->print();
				break;

			case 5:
				dynamicForest->printTrees();
				break;
		}
	}

}

int main(){
	decode();
}