#ifndef GENETIC_OPENMP_H
#define GENETIC_OPENMP_H

#include <vector>
#include <random>

std::vector<int> geneticOpenMP(std::vector<std::vector<int>> &adj_matrix, 
                               int n, 
                               bool must_have_nodes, 
                               std::vector<int>& must_nodes, 
                               std::mt19937 &main_engine);

#endif // GENETIC_OPENMP_H