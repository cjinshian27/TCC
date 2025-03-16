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
}