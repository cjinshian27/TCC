#include <iostream>
#include <vector>
#include <algorithm>
#include "decrementalMSF.hpp"
#include <chrono>

/*
runs in O(mlg(m)) time, since we are sorting 
the edges in increasing weight
*/ 

DecrementalMSF<int> * buildDecrementalMSF(){
	
	unsigned int numberOfVertices;
	unsigned int numberOfEdges;
	unsigned int u, v, weight;
	
	std::cin >> numberOfVertices;
	std::vector<int> vertices(numberOfVertices);

	for(unsigned int i = 0; i < numberOfVertices; ++i){
		vertices[i] = i;
	}

	std::cin >> numberOfEdges;
	std::vector<std::tuple<int, int, int>> edges(numberOfEdges);

	for(unsigned int i = 0; i < numberOfEdges; ++i){
		std::cin >> u >> v >> weight; 
		edges[i] = {u, v, weight};
	}
	
	// sort the edges by increasing weight
	std::sort(edges.begin(), edges.end(), 
	[](const auto& a, const auto& b) {
		return std::get<2>(a) < std::get<2>(b);
	});
	
	DecrementalMSF<int> * decrementalMSF = new DecrementalMSF<int>(vertices);
	
	// add the edges beforehand (user is not allowed to add more later)
	for (const auto& [u, v, weight] : edges) {
		decrementalMSF->add(u, v, weight);
    }
	
	return decrementalMSF;
}

//decode function
void decode(){

	int operation, u, v;

	DecrementalMSF<int> * decrementalMSF = buildDecrementalMSF(); 

	if(!decrementalMSF){
		return;
	} 
	
	int totalRemoved = 0;
	int batch = 100;
	auto start = std::chrono::steady_clock::now();

	while(std::cin >> operation){
		

		if(totalRemoved > 25600) break;

		if(totalRemoved == batch){
			auto end = std::chrono::steady_clock::now();
    		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

			std::cout << "Elapsed time: " << elapsed.count() << " milliseconds for batch " << batch << " - MST weight: " <<  decrementalMSF->getTotalWeight() << std::endl;
		
			batch = batch + batch;
		}

		switch(operation){

			case 1:
				std::cin >> u >> v;
				if(decrementalMSF->areConnected(u, v)){
					std::cout << u << " and " << v << " are connected!" << '\n';
				} 
				else{
					std::cout << u << " and " << v << " are NOT connected!" << '\n';
				} 
				break;
			
			case 2: 
				std::cin >> u >> v;
				++totalRemoved;
				decrementalMSF->remove(u, v);
				break;
			
			case 3:
				std::cout << decrementalMSF->getTotalWeight() << std::endl;
				break;

			case 4:
				decrementalMSF->printWeightedEdges();
				break;

			case 5:
				decrementalMSF->printForests();
				break;

			case 6: 
				decrementalMSF->printAdjancencyLists();
				break;
		}
	}
}

int main(){
	decode();
}