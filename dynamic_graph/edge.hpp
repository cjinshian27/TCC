template<typename Key>

class Edge{

	private:
		unsigned int level;
		Key u;
		Key v;

	public:

		void setLevel(unsigned int level){
			
			this->level = level;
		}

		unsigned int getLevel(){
			
			return this->level;
		}

		bool isLevel(unsigned int level){

			return this->level == level;
		}
}