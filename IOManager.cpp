#include "IOManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

using namespace std;

Config readConfig(const string& filename) {
    Config cfg;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Brak pliku " << filename << ". Uzywam domyslnych ustawien." << endl;
        return { true, true, 1, {5, 10}, 1, {}, "results.csv" };
    }
    string line, key;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        stringstream ss(line);
        ss >> key;
        if (key == "enable_progress") ss >> cfg.enable_progress;
        else if (key == "use_upper_bound") ss >> cfg.use_upper_bound;
        else if (key == "num_repetitions") ss >> cfg.num_repetitions;
        else if (key == "num_instances_per_size") ss >> cfg.num_instances_per_size;
        else if (key == "output_file") ss >> cfg.output_file;
        else if (key == "sizes") {
            int s;
            while (ss >> s) cfg.sizes.push_back(s);
        }
        else if (key == "tsplib_files") {
            string f;
            while (ss >> f) cfg.tsplib_files.push_back(f);
        }
    }
    return cfg;
}

vector<vector<int>> readGraphFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return {};
    int n;
    if (!(file >> n)) return {};
    vector<vector<int>> graph(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            file >> graph[i][j];
    return graph;
}

vector<vector<int>> generateGraph(int n, bool symmetric) {
    vector<vector<int>> graph(n, vector<int>(n, 0));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 100);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                int cost = dist(gen);
                graph[i][j] = cost;
                if (symmetric) graph[j][i] = cost;
            }
            else graph[i][j] = -1;
        }
    }
    return graph;
}