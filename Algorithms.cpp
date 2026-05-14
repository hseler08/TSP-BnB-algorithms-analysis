#include "Algorithms.h"
#include <queue>
#include <stack>
#include <chrono>
#include <algorithm>

using namespace std;

//HEURYSTYKA NEAREST NEIGHBOR (Dla UB)
pair<int, vector<int>> nearestNeighbor(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<bool> visited(n, false);
    vector<int> path;
    int current = 0;
    int total_cost = 0;

    path.push_back(0);
    visited[0] = true;

    for (int i = 1; i < n; i++) {
        int next_city = -1;
        int min_edge = INF;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && graph[current][j] != -1 && graph[current][j] < min_edge) {
                min_edge = graph[current][j];
                next_city = j;
            }
        }
        if (next_city == -1) return { INF, {} };
        visited[next_city] = true;
        total_cost += min_edge;
        current = next_city;
        path.push_back(next_city);
    }
    if (graph[current][0] == -1) return { INF, {} };
    total_cost += graph[current][0];
    path.push_back(0);
    return { total_cost, path };
}

//REDUKCJA MACIERZY (Dla LB)
int reduceMatrix(vector<vector<int>>& matrix) {
    int n = matrix.size();
    int reduction_cost = 0;
    for (int i = 0; i < n; i++) {
        int row_min = INF;
        for (int j = 0; j < n; j++) if (matrix[i][j] < row_min) row_min = matrix[i][j];
        if (row_min != INF && row_min > 0) {
            reduction_cost += row_min;
            for (int j = 0; j < n; j++) if (matrix[i][j] != INF) matrix[i][j] -= row_min;
        }
    }
    for (int j = 0; j < n; j++) {
        int col_min = INF;
        for (int i = 0; i < n; i++) if (matrix[i][j] < col_min) col_min = matrix[i][j];
        if (col_min != INF && col_min > 0) {
            reduction_cost += col_min;
            for (int i = 0; i < n; i++) if (matrix[i][j] != INF) matrix[i][j] -= col_min;
        }
    }
    return reduction_cost;
}

//TWORZENIE DZIECKA
Node createChildNode(const vector<vector<int>>& parent_matrix, const vector<int>& parent_path, int parent_bound, int from, int to, int level) {
    Node child;
    int n = parent_matrix.size();
    child.reduced_matrix = parent_matrix;
    child.path = parent_path;
    child.path.push_back(to);
    child.level = level;
    child.vertex = to;
    int edge_cost = parent_matrix[from][to];
    for (int i = 0; i < n; i++) {
        child.reduced_matrix[from][i] = INF;
        child.reduced_matrix[i][to] = INF;
    }
    child.reduced_matrix[to][0] = INF;
    int new_reduction = reduceMatrix(child.reduced_matrix);
    child.bound = (edge_cost == INF) ? INF : parent_bound + edge_cost + new_reduction;
    return child;
}

//BFS
Result solveBFS(const vector<vector<int>>& graph, bool use_ub) {
    int n = graph.size();
    Result res = { 0.0, 0, INF, {} };
    auto start = chrono::high_resolution_clock::now();

    int current_ub = INF;
    if (use_ub) {
        auto nn = nearestNeighbor(graph);
        current_ub = nn.first;
        res.best_path = nn.second;
    }
    res.optimal_cost = current_ub;

    Node root;
    root.reduced_matrix = graph;
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) if (root.reduced_matrix[i][j] == -1) root.reduced_matrix[i][j] = INF;
    root.path = { 0 }; root.level = 1; root.vertex = 0;
    root.bound = reduceMatrix(root.reduced_matrix);

    queue<Node> q;
    if (root.bound < current_ub) q.push(root);
    size_t max_mem = 0;

    while (!q.empty()) {
        max_mem = max(max_mem, q.size());
        Node curr = q.front(); q.pop();
        if (curr.bound >= current_ub) continue;
        if (curr.level == n) {
            if (curr.bound < current_ub) {
                current_ub = curr.bound;
                res.optimal_cost = current_ub;
                res.best_path = curr.path;
                res.best_path.push_back(0);
            }
            continue;
        }
        for (int i = 0; i < n; i++) {
            if (curr.reduced_matrix[curr.vertex][i] != INF) {
                Node child = createChildNode(curr.reduced_matrix, curr.path, curr.bound, curr.vertex, i, curr.level + 1);
                if (child.bound < current_ub) q.push(child);
            }
        }
    }
    res.time_ms = chrono::duration<double, milli>(chrono::high_resolution_clock::now() - start).count();
    res.max_memory_nodes = max_mem;
    return res;
}

//DFS
Result solveDFS(const vector<vector<int>>& graph, bool use_ub) {
    int n = graph.size();
    Result res = { 0.0, 0, INF, {} };
    auto start = chrono::high_resolution_clock::now();
    int current_ub = INF;
    if (use_ub) {
        auto nn = nearestNeighbor(graph);
        current_ub = nn.first;
        res.best_path = nn.second;
    }
    res.optimal_cost = current_ub;

    Node root;
    root.reduced_matrix = graph;
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) if (root.reduced_matrix[i][j] == -1) root.reduced_matrix[i][j] = INF;
    root.path = { 0 }; root.level = 1; root.vertex = 0;
    root.bound = reduceMatrix(root.reduced_matrix);

    stack<Node> s;
    if (root.bound < current_ub) s.push(root);
    size_t max_mem = 0;

    while (!s.empty()) {
        max_mem = max(max_mem, s.size());
        Node curr = s.top(); s.pop();
        if (curr.bound >= current_ub) continue;
        if (curr.level == n) {
            if (curr.bound < current_ub) {
                current_ub = curr.bound;
                res.optimal_cost = current_ub;
                res.best_path = curr.path;
                res.best_path.push_back(0);
            }
            continue;
        }
        for (int i = 0; i < n; i++) {
            if (curr.reduced_matrix[curr.vertex][i] != INF) {
                Node child = createChildNode(curr.reduced_matrix, curr.path, curr.bound, curr.vertex, i, curr.level + 1);
                if (child.bound < current_ub) s.push(child);
            }
        }
    }
    res.time_ms = chrono::duration<double, milli>(chrono::high_resolution_clock::now() - start).count();
    res.max_memory_nodes = max_mem;
    return res;
}

//BEST-FIRST
Result solveBestFirst(const vector<vector<int>>& graph, bool use_ub) {
    int n = graph.size();
    Result res = { 0.0, 0, INF, {} };
    auto start = chrono::high_resolution_clock::now();
    int current_ub = INF;
    if (use_ub) {
        auto nn = nearestNeighbor(graph);
        current_ub = nn.first;
        res.best_path = nn.second;
    }
    res.optimal_cost = current_ub;

    Node root;
    root.reduced_matrix = graph;
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) if (root.reduced_matrix[i][j] == -1) root.reduced_matrix[i][j] = INF;
    root.path = { 0 }; root.level = 1; root.vertex = 0;
    root.bound = reduceMatrix(root.reduced_matrix);

    priority_queue<Node, vector<Node>, greater<Node>> pq;
    if (root.bound < current_ub) pq.push(root);
    size_t max_mem = 0;

    while (!pq.empty()) {
        max_mem = max(max_mem, pq.size());
        Node curr = pq.top(); pq.pop();
        if (curr.bound >= current_ub) continue;
        if (curr.level == n) {
            if (curr.bound < current_ub) {
                current_ub = curr.bound;
                res.optimal_cost = current_ub;
                res.best_path = curr.path;
                res.best_path.push_back(0);
            }
            continue;
        }
        for (int i = 0; i < n; i++) {
            if (curr.reduced_matrix[curr.vertex][i] != INF) {
                Node child = createChildNode(curr.reduced_matrix, curr.path, curr.bound, curr.vertex, i, curr.level + 1);
                if (child.bound < current_ub) pq.push(child);
            }
        }
    }
    res.time_ms = chrono::duration<double, milli>(chrono::high_resolution_clock::now() - start).count();
    res.max_memory_nodes = max_mem;
    return res;
}

Result solveTSP(const vector<vector<int>>& graph, SearchMethod method, bool use_ub) {
    if (method == BFS) return solveBFS(graph, use_ub);
    if (method == DFS) return solveDFS(graph, use_ub);
    return solveBestFirst(graph, use_ub);
}