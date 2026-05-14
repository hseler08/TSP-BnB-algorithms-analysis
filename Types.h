#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
#include <limits>

const int INF = std::numeric_limits<int>::max();

enum SearchMethod { BFS, DFS, BEST_FIRST };

struct Config {
    bool enable_progress;
    bool use_upper_bound;
    int num_repetitions;
    std::vector<int> sizes;
    int num_instances_per_size;
    std::vector<std::string> tsplib_files;
    std::string output_file;
};

struct Node {
    std::vector<std::vector<int>> reduced_matrix;
    std::vector<int> path;
    int cost;
    int bound;
    int level;
    int vertex;

    bool operator>(const Node& other) const {
        return bound > other.bound;
    }
};

struct Result {
    double time_ms;
    size_t max_memory_nodes;
    int optimal_cost;
    std::vector<int> best_path;
};

#endif