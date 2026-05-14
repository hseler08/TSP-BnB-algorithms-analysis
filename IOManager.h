#ifndef IOMANAGER_H
#define IOMANAGER_H

#include "Types.h"

Config readConfig(const std::string& filename);
std::vector<std::vector<int>> readGraphFromFile(const std::string& filename);
std::vector<std::vector<int>> generateGraph(int n, bool symmetric);

#endif