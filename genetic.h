#include <vector>
#include <random>
#include <math.h>
#include <algorithm>

#include <iostream>

// Initialization
/*
    Creating population and filling it with random chromosomes.
    Chromosome -> posssible solution
    Chromosome is made of genes. Gene can have value:
        1 -> vertice is a part of the cover
        0 -> vertice is not a part of the cover
*/

std::vector<std::vector<int>> initialization(int n, int max_population, std::mt19937 engine, bool must_have_nodes, std::vector<int> must_nodes) {
    std::uniform_int_distribution<int> dist{0,1};
    
    std::vector<std::vector<int>> population;
    population.resize(max_population);

    for (int i = 0; i < max_population; i++){   // iterating through chromosomes
        for (int j = 0; j < n; j++){            // iterating through genes
            // setting constant genes if there are vertices to include in the cover
            if (must_have_nodes) {
                if (std::find(must_nodes.begin(), must_nodes.end(), j) != must_nodes.end()) {
                    population[i].push_back(1);
                } else {
                    population[i].push_back(dist(engine));
                }

            } else {
                population[i].push_back(dist(engine));
            }    
        }
    }
    return population;
}


double fitness(std::vector<int> chromosome, std::vector<std::vector<int>> &adj_matrix, int n, int number_of_edges) {

    std::vector<std::vector<int>> fitness_matrix;
    fitness_matrix.resize(n);
    for (int i = 0; i < n; i++) {
        fitness_matrix[i].resize(n);
    }

    double fitness_count = 0;    
    double penalty = 0;
    for(int gene = 0; gene < chromosome.size(); gene++) {
        if (chromosome[gene] == 1) {
            for (int j = 0; j < n; j++)  {
                if (adj_matrix[gene][j] == 1) {
                    if (fitness_matrix[gene][j] == 1) {
                        penalty += 1;
                    } else {
                        fitness_count += 1;
                    }
                    fitness_matrix[gene][j] = 1;
                    fitness_matrix[j][gene] = 1;
                    
                } else {
                    fitness_matrix[gene][j] = 0;
                    fitness_matrix[j][gene] = 0;
                }
            }
        } 
    }

    fitness_count = fitness_count/number_of_edges*100 - penalty / number_of_edges;
    return fitness_count;
}


std::vector<int> crossover(std::vector<int> parentA, std::vector<int> parentB, std::mt19937 engine) {
    std::vector<int> child;
    int size = parentA.size();

    std::uniform_int_distribution<int> mid(0, size-1);

    int midpoint = mid(engine);

    for (int gene = 0; gene < size; gene++) {
        if (gene > midpoint) {
            child.push_back(parentB[gene]);
        } else {
            child.push_back(parentA[gene]);
        }
    }

    return child;
    
}

std::vector<int> mutate(std::vector<int> child, double mutation_probability, std::mt19937 engine, bool must_have_nodes, std::vector<int> must_nodes) {

    std::uniform_real_distribution<> mut(0,1);
    double mutation_random;
    std::uniform_int_distribution<int> gen_mut{0,1};

    for (int gene = 0; gene < child.size(); gene++) {
        if (must_have_nodes && std::find(must_nodes.begin(), must_nodes.end(), gene) != must_nodes.end()) {
            continue;
        }
        mutation_random = mut(engine);
        if (mutation_random < mutation_probability) {
            child[gene] = gen_mut(engine);
        }
    }
    return child;
}



std::vector<std::vector<int>> selection(std::vector<std::vector<int>> population, std::vector<double> population_fitness, std::mt19937 engine, double mutation_probability, int n, bool must_have_nodes, std::vector<int> must_nodes) {

    std::vector<std::vector<int>> new_population;

    for (int chrom = 0; chrom < population.size(); chrom++) {
        
        std::discrete_distribution<> d(population_fitness.begin(), population_fitness.end());

        std::vector<int> parentA = population[d(engine)];
        std::vector<int> parentB = population[d(engine)];

        std::vector<int> child = crossover(parentA, parentB, engine);
        child = mutate(child, mutation_probability, engine, must_have_nodes, must_nodes);

        new_population.push_back(child);
    } 

    return new_population;
}


std::vector<int> genetic(std::vector<std::vector<int>> &adj_matrix, int n, bool must_have_nodes, std::vector<int> must_nodes, std::mt19937 &engine) {
    // std::random_device rd{};
    // std::mt19937 engine{rd()};

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
    population = initialization(n, max_population, engine, must_have_nodes, must_nodes);

    // fitness **********************************
    std::vector<double> population_fitness;
    population_fitness.resize(max_population);

    // looping till all generations are generated
    for (int gen = 0; gen < max_generations-1; gen++) {

        // calculating fitness scores
        for (int i = 0; i < max_population; i++) {
            population_fitness[i] = fitness(population[i], adj_matrix, n, number_of_edges);
        }

        // selection & reproduction **********************************
        population = selection(population, population_fitness, engine, mutation_rate, n, must_have_nodes, must_nodes);
        generations++;
    }

    // calculating fitness scores for the last population
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