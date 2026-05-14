#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include "Types.h"
#include "IOManager.h"
#include "Algorithms.h"

using namespace std;

string toExcel(double value) {
    string s = to_string(value);
    s.erase(s.find_last_not_of('0') + 1, string::npos);
    if (s.back() == '.') s.pop_back();
    replace(s.begin(), s.end(), '.', ',');
    return s;
}

int main() {
    cout << "--- START PROGRAMU ---" << endl;
    Config cfg = readConfig("config.txt");

    if (cfg.sizes.empty() && cfg.tsplib_files.empty()) {
        cfg.sizes = { 5, 6, 7 };
        cfg.num_instances_per_size = 1;
    }

    ofstream out(cfg.output_file);
    if (!out.is_open()) {
        cout << "Blad otwarcia pliku wynikowego!" << endl;
        return 1;
    }

    out << "Rozmiar;Typ;ID;UB_Mode;BFS_Time;BFS_Nodes;DFS_Time;DFS_Nodes;BEST_Time;BEST_Nodes;Koszt\n";

    string ub_status = cfg.use_upper_bound ? "Z_UB" : "BEZ_UB";

    if (cfg.enable_progress) {
        cout << string(100, '=') << endl;
        cout << "TRYB: " << ub_status << " (NN)" << endl;
        cout << string(100, '-') << endl;
    }

    //BADANIA NA PLIKACH
    for (const string& fileName : cfg.tsplib_files) {
        vector<vector<int>> graph = readGraphFromFile(fileName);

        if (graph.empty()) {
            cout << "[!] Nie mozna otworzyc lub odczytac pliku: " << fileName << endl;
            continue;
        }

        int size = graph.size();
        Result rBFS = solveTSP(graph, BFS, cfg.use_upper_bound);
        Result rDFS = solveTSP(graph, DFS, cfg.use_upper_bound);
        Result rBEST = solveTSP(graph, BEST_FIRST, cfg.use_upper_bound);

        out << size << ";" << fileName << ";1;" << ub_status << ";"
            << toExcel(rBFS.time_ms) << ";" << rBFS.max_memory_nodes << ";"
            << toExcel(rDFS.time_ms) << ";" << rDFS.max_memory_nodes << ";"
            << toExcel(rBEST.time_ms) << ";" << rBEST.max_memory_nodes << ";"
            << rBFS.optimal_cost << "\n";

        if (cfg.enable_progress) {
            cout << left << setw(8) << size << setw(20) << fileName.substr(0, 18) << setw(5) << "FILE"
                << "| BFS: " << setw(15) << rBFS.time_ms
                << "| DFS: " << setw(15) << rDFS.time_ms
                << "| BEST: " << setw(15) << rBEST.time_ms << "| " << rBFS.optimal_cost << endl;
        }
    }

    //BADANIA NA INSTANCJACH LOSOWYCH
    for (int size : cfg.sizes) {
        for (int i = 0; i < cfg.num_instances_per_size; ++i) {
            for (int type = 0; type < 2; ++type) {
                bool isSym = (type == 0);
                string typeName = isSym ? "TSP" : "ATSP";
                vector<vector<int>> graph = generateGraph(size, isSym);

                Result rBFS = solveTSP(graph, BFS, cfg.use_upper_bound);
                Result rDFS = solveTSP(graph, DFS, cfg.use_upper_bound);
                Result rBEST = solveTSP(graph, BEST_FIRST, cfg.use_upper_bound);

                out << size << ";" << typeName << ";" << (i + 1) << ";" << ub_status << ";"
                    << toExcel(rBFS.time_ms) << ";" << rBFS.max_memory_nodes << ";"
                    << toExcel(rDFS.time_ms) << ";" << rDFS.max_memory_nodes << ";"
                    << toExcel(rBEST.time_ms) << ";" << rBEST.max_memory_nodes << ";"
                    << rBFS.optimal_cost << "\n";

                if (cfg.enable_progress) {
                    cout << left << setw(8) << size << setw(20) << typeName << setw(5) << (i + 1)
                        << "| BFS: " << setw(15) << rBFS.time_ms
                        << "| DFS: " << setw(15) << rDFS.time_ms
                        << "| BEST: " << setw(15) << rBEST.time_ms << "| " << rBFS.optimal_cost << endl;
                }
            }
        }
    }

    out.close();
    cout << string(100, '=') << endl;
    cout << "Zakonczone. Wyniki dopisane do: " << cfg.output_file << endl;
    return 0;
}