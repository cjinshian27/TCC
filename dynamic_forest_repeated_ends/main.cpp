#include <iostream>
#include <utility>
#include <unordered_map>
#include <vector>
#include <string>
#include <iterator>
#include <set>
#include "dynamicForest.hpp"

//decode function
void decode(){

	int operation, u, v;
	unsigned int n;

	std::cin >> n;
	std::vector<int> vertices(n);

	for(unsigned int i = 1; i <= n; ++i){
		vertices[i-1] = i;
	}

	DynamicForest<int> * dynamicForest = new DynamicForest<int>(vertices);

	while(std::cin >> operation){
		
		switch(operation){
			
			case 1:
				std::cin >> u >> v;
				if(dynamicForest->isConnected(u, v)) std::cout << "True" << '\n';
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