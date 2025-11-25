struct GraphEdge {
    int u, v, weight;
    
    GraphEdge(int u, int v, int w) : u(u), v(v), weight(w) {}
    
    bool operator<(const GraphEdge& other) const {
        return weight < other.weight;
    }
};