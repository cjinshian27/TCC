#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

template<typename Key>

class MinHeap {
    private:
        // stores (v, weight), where v is a neighbor
        std::vector<std::pair<Key, int>> heap; 

        // stores the position of v in the heap
        std::unordered_map<Key, int> position;

        //heapify up in O(lg(n)) time
        void heapifyUp(int i) {
            while (i > 0) {
                int parent = (i - 1) / 2;
                if (heap[i].second < heap[parent].second) {
                    std::swap(heap[i], heap[parent]);
                    position[heap[i].first] = i;
                    position[heap[parent].first] = parent;
                    i = parent;
                } else break;
            }
        }
        
        //heapify down in O(lg(n)) time
        void heapifyDown(int i) {
            int n = heap.size();
            int left, right, smallest;
            
            while (true) {
                left = 2 * i + 1; 
                right = 2 * i + 2; 
                smallest = i;
                
                if (left < n && heap[left].second < heap[smallest].second)
                    smallest = left;
                if (right < n && heap[right].second < heap[smallest].second)
                    smallest = right;
                if (smallest != i) {
                    std::swap(heap[i], heap[smallest]);
                    position[heap[i].first] = i;
                    position[heap[smallest].first] = smallest;
                    i = smallest;
                } else break;
            }
        }

    public:

        // checks if heap is empty in O(1) time
        bool isEmpty() const {
            return heap.empty();
        }

        // returns the heap size in O(1) time
        unsigned int size() {
            return heap.size();
        }
        
        // gets the pair <v, weight> from heap in O(1) time
        std::pair<Key, int> getMin() {
            return heap[0];
        }
        
        // inserts a (v, weight) neighbor into heap in O(lg(n)) time
        void insert(Key v, int weight) {
            if (position.count(v)) {
                return;
            }
            heap.push_back({v, weight});
            long unsigned int index = heap.size() - 1;
            position[v] = index;
            heapifyUp(index);
        }
        
        // removes a neighbor v in O(lg(n)) time
        void remove(Key v) {
            if (!position.count(v)) return;
            
            long unsigned int index = position[v];
            position.erase(v);
            
            if (index == heap.size() - 1) {
                heap.pop_back();
                return;
            }
            
            heap[index] = heap.back();
            heap.pop_back();
            position[heap[index].first] = index;
            heapifyDown(index);
            heapifyUp(index);
        }

        // extracts the lighest edge in O(lg(n)) time
        std::pair<Key, int> extractMin(){
            std::pair<Key, int> extractedNeighbor = heap[0];
            remove(heap[0].first);
            return extractedNeighbor;
        }

        // prints in O(n) time (for debugging purposes)
        void print() {
            for (auto [v, w] : heap)
                std::cout << " (" << v << ", w=" << w << ")";
            std::cout << "\n";
        }
};