#include <iostream>
#include <vector>
#include <chrono>
#include <random>

#include "nlohmann/json.hpp"

#include "generateGraph.h"
#include "json.h"
#include "genetic.h"
// #include "geneticOpenMP.h"

using json = nlohmann::json;


int n = 5;          // number of nodes
double p = 0.4;     // graph density

bool save = true;
bool read = true;
bool saveResult = true;

bool must_have_nodes = true;

std::vector<int> must_nodes;
std::vector<std::vector<int>> adj_matrix;

std::string inputName = "inputEvery";
std::string inputPath = "inputs/" + inputName + ".json";


void graph(std::mt19937 &generator, std::string instanceName) {

    // -------- creating adjacency matrix -------- //

    
    adj_matrix.resize(n);
    for (int i = 0; i < adj_matrix.size(); i++) {
        adj_matrix[i].resize(n);
    }

    // -------- generating graph -------- //

    // printf("generating graph");

    

    generateErdosRenyiGraph(n, p, adj_matrix, generator);

    // printf("generating nodes");

    must_nodes = generateMustHaveNodes(n, generator);


    // -------- saving graph to JSON -------- //

    if (save) {
        // printf("saving graph");
        

        saveAdjToJSON(adj_matrix, n, must_have_nodes, must_nodes, instanceName);
    }


    // -------- reading graph from JSON -------- //

    if (read) {

        // printf("reading graph");

        json data = readJSON("graph.json");

        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++) {
                adj_matrix[i][j] = (int)data[std::to_string(i)][j];
            }
        }

        must_nodes.clear();

        for (int i = 0; i < (int)data["-1"].size(); i++) {
            must_nodes.push_back((int)data["-1"][i]);
        }

    }

}




int main() {
    
    // -------- reading config data from json -------- //

    std::random_device rd{};
    std::mt19937 generator{rd()};

    
    json config = readJSON("config.json");
    save = config["save"];
    read = config["read"];
    saveResult = config["saveResult"];


    // -------- reading input data from json -------- //

    json input = readJSON(inputPath);
    // std::cout << input.dump(4) << std::endl;

    json output;

    for (auto it = input.begin(); it != input.end(); ++it) {
        std::string key = it.key();
        auto& value = it.value();

        // std::cout << "value = " << value.dump() << std::endl;
    
        n = value["nodes"];
        p = value["density"];

        printf("n: %d", n);
        printf("p: %f", p);


        std::string instanceName = "n" + std::to_string(n)+ "p0" + std::to_string(int(p*10));

        graph(generator, instanceName);



        // ---- GENETYCZNY --- //

        // printf("genetic");

        std::vector<int> minCover;

        for (int i = 0; i < 1; ++i) {

            std::cout << "\n";
            auto begin = std::chrono::high_resolution_clock::now();  

            minCover = genetic(adj_matrix, n, must_have_nodes, must_nodes, generator);

            // minCover = geneticOpenMP(adj_matrix, n, must_have_nodes, must_nodes, generator);

            auto end = std::chrono::high_resolution_clock::now();
            int time = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
            std::cout << time << " microseconds" << std::endl;

            

            if (i == 0) {
                output[key]["nodes"] = n;
                output[key]["density"] = p;
            }
            
            output[key]["time" + std::to_string(i)] = time;

        }


        std::ofstream writer;
        writer.open("outputs/output.json");
        writer << output.dump(4);
        


        if (saveResult) {
            std::string instanceName = "n" + std::to_string(n)+ "p0" + std::to_string(int(p*10));
            std::string coverFile = "outputs/covers/" + instanceName + ".json"; 
            saveCover(minCover, coverFile);
        }



    }


    
    

    return 0;
}
