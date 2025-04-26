#include "geneticOpenMP.h"
#include <vector>
#include <random>
#include <math.h>
#include <algorithm>
#include <omp.h>
#include "genetic.h"


std::vector<int> geneticOpenMP(std::vector<std::vector<int>> &adj_matrix, int n, bool must_have_nodes, std::vector<int> &must_nodes, std::mt19937 &main_engine) {

    int max_population = 30;
    float mutation_rate = 0.1;
    int generations = 1;
    int max_generations = 10;

    // counting edges in graph for later use in fitness function
    int number_of_edges = 0;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            if (adj_matrix[i][j] == 1) {
                number_of_edges++;
            }
        }
    }

    // first population *************************
    std::vector<std::vector<int>> population;
    population = initialization(n, max_population, main_engine, must_have_nodes, must_nodes);

    // fitness **********************************
    // std::vector<double> population_fitness;
    // population_fitness.resize(max_population);
    std::vector<double> population_fitness(max_population, 0.0);

    // looping till all generations are generated
    for (int gen = 0; gen < max_generations-1; gen++) {


        #pragma omp parallel for default(none) shared(population, population_fitness, adj_matrix, number_of_edges, n, max_population)
        // calculating fitness scores
        for (int i = 0; i < max_population; i++) {
            population_fitness[i] = fitness(population[i], adj_matrix, n, number_of_edges);
        }

        // selection & reproduction **********************************
        // population = selection(population, population_fitness, engine, mutation_rate, n, must_have_nodes, must_nodes);
        // generations++;


        std::vector<std::vector<int>> new_population;
        new_population.reserve(max_population);

        #pragma omp parallel for default(none) shared(population, population_fitness, new_population, mutation_rate, n, must_have_nodes, must_nodes, max_population)
        for (int chrom = 0; chrom < population.size(); chrom++) {
            // local generator
            std::random_device rd_local;
            std::mt19937 local_engine(rd_local() + omp_get_thread_num());

            std::discrete_distribution<> d(population_fitness.begin(), population_fitness.end());

            std::vector<int> parentA = population[d(local_engine)];
            std::vector<int> parentB = population[d(local_engine)];

            std::vector<int> child = crossover(parentA, parentB, local_engine);
            child = mutate(child, mutation_rate, local_engine, must_have_nodes, must_nodes);

            #pragma omp critical
            {
                new_population.push_back(child);
            }
        } 

        population = new_population;

    }

    // calculating fitness scores for the last population
    #pragma omp parallel for default(none) shared(population, population_fitness, adj_matrix, number_of_edges, n, max_population)
    for (int i = 0; i < max_population; i++) {
        population_fitness[i] = fitness(population[i], adj_matrix, n, number_of_edges);
    }

    // picking chromosome with highest fitness score
    std::vector<double> max_fit = {-1, -1};
    for (int fit = 0; fit < population_fitness.size(); fit++) {
        if (population_fitness[fit] > max_fit[0]) {
            max_fit[0] = population_fitness[fit];
            max_fit[1] = fit;
        } 
    }

    // changing 'chromosome notation' to vertices
    int max_fit_int = (int)max_fit[1];
    std::vector<int> min_cover;
    for (int i = 0; i < population[max_fit_int].size(); i++) {
        if (population[max_fit_int][i] == 1) {
            min_cover.push_back(i);
        }
    }
 
    return min_cover;
}