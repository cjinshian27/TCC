#include "../dynamic_forest/dynamic_forest.hpp"

template<typename Key>

class Forest{

	private:
		DynamicForest<Key> * forest;

	public:

		Forest(){
			this->forest = new DynamicForest<Key>();
		}

		~Forest(){
		}

		bool isConnected(Key u, Key v){
			return this->forest->isConnected(u, v);
		}

		void add(Key u, Key v){
			this->forest->link(u, v);
		}

		bool hasEdge(Key u, Key v){
			return this->forest->hasEdge(u, v);
		}
}