#include <iostream>
#include <vector>
#include <algorithm>
#include "node.hpp"
#include "splayTree.hpp"

void decode(){
	
	int operation, key;

	/*
	splayTree1 was created primarily for testing the test case 7. 
	splayTree is used for testing the remaining test cases, which 
	are presented in the assignment's wording. 
	*/
	
	SplayTree<int> * splayTree1 = new SplayTree<int>();
	splayTree1->insert(15);
	splayTree1->insert(12);

	SplayTree<int> * splayTree = new SplayTree<int>();

	std::pair<SplayTree<int> *, SplayTree<int> *> pairOfSplayTrees;

	//read the input
	while(std::cin >> operation){
		
		std::cout << "===========" << '\n';
		
		switch(operation){
			case 1:
				std::cin >> key;
				splayTree->insert(key);
				break;
			
			case 2:	
				std::cin >> key;
				splayTree->remove(key);
				break;
			
			case 3:
				std::cin >> key;
				if(splayTree->search(key)){
					std::cout << 1 << '\n';
				}
				else{
					std::cout << 0 << '\n';
 				}
				break;
			
			case 4:
				std::cout << splayTree->min() << '\n';
				break;
			
			case 5:
				splayTree->print();
				break;
			
			case 6:
				std::cin >> key;
				pairOfSplayTrees = splayTree->split(key);
				pairOfSplayTrees.first->print();
				pairOfSplayTrees.second->print();
				break;
			
			case 7: //this extra test case is for join
				splayTree->join(splayTree1);
				break;
		}
	}

	delete splayTree;
	delete splayTree1;
	splayTree = nullptr;
	splayTree1 = nullptr;
}


int main(){

	decode();
}