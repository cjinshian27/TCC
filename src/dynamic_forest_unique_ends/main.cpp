#include <iostream>
#include "dynamicForest.hpp"

//decode function
void decode(){

	int operation, n, u, v;

	DynamicForest<int> * dynamicForest;

	while(std::cin >> operation){
		
		switch(operation){
			
			case 1:
				std::cin >> n;
				dynamicForest = new DynamicForest<int>(n);
				break;

			case 2:
				std::cin >> u >> v;
				if(dynamicForest->areConnected(u, v)) std::cout << "True" << '\n';
				else std::cout << "False" << '\n';
				break;
			
			case 3:
				std::cin >> u >> v;
				dynamicForest->link(u, v);
				break;
			
			case 4:
				std::cin >> u >> v;
				dynamicForest->cut(u, v);
				break;
			
			case 5:
				dynamicForest->print();
				break;

			case 6:
				dynamicForest->printTrees();
				break;
		}
	}

}

int main(){
	decode();
}