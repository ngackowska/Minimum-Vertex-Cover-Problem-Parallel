#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <math.h>



void generateErdosRenyiGraph(int n, double p, std::vector<std::vector<int>> &adj_matrix, std::mt19937 &generator) {
    std::uniform_real_distribution<double> ge(0,1);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double randProb = ge(generator);

            if (i == j) {
                adj_matrix[i][j] = 0;
            } else if (randProb < p) {
                adj_matrix[i][j] = 1;
                adj_matrix[j][i] = 1;
            } else {
                adj_matrix[i][j] = 0;
                adj_matrix[j][i] = 0;
            }

        }
    }
}


std::vector<int> generateMustHaveNodes(int n, std::mt19937 &engine) {
    std::uniform_int_distribution<> cnt{1, (int)ceil(0.2*n)};
    int must_nodes_count = cnt(engine);

    std::vector<int> must_have_nodes(2*must_nodes_count+1);
    std::iota(begin(must_have_nodes), end(must_have_nodes)-1, 0);
    std::random_shuffle(begin(must_have_nodes), end(must_have_nodes));
    must_have_nodes.resize(must_nodes_count);

    return must_have_nodes;
}