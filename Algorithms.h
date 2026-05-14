#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Types.h"
#include <vector>

Result solveBFS(const std::vector<std::vector<int>>& graph, bool use_ub);
Result solveDFS(const std::vector<std::vector<int>>& graph, bool use_ub);
Result solveBestFirst(const std::vector<std::vector<int>>& graph, bool use_ub);
Result solveTSP(const std::vector<std::vector<int>>& graph, SearchMethod method, bool use_ub);

std::pair<int, std::vector<int>> nearestNeighbor(const std::vector<std::vector<int>>& graph);
int reduceMatrix(std::vector<std::vector<int>>& matrix);

#endif