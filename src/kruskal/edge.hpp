struct Edge {
    int u, v, weight;
    
    Edge(int u, int v, int w) : u(u), v(v), weight(w) {}
    
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};