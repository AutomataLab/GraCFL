
// include this header if you want to print the exception message
#include <iostream>
#include <vector>
#include <unordered_set>

// As the function throws error, we need to include the stdexcept header
#include <stdexcept>

/*
* include this two headers to use the GraCFL api
*/
#include "solvers/Solver.hpp"
#include "utils/Config.hpp"

/*
* @brief Example function demonstrating the usage of the GraCFL api.
* 
* This function initializes a configuration object, creates a Solver instance,
* and executes the CFL-reachability analysis. It handles exceptions and prints
* usage instructions in case of errors.
* Note: The function name 'example' is just a placeholder and can be changed as needed.
* The code snippet below can be added to a larger program or used as a standalone example.
* It is assumed that the necessary headers and namespaces are included at the beginning of the file.
*/      
void example() {
    try {
        // Create a configuration object
        gracfl::Config config;
        // Set the configuration parameters

        // graphFilepath and grammarFilepath are required parameters
        config.graphFilepath = "path/to/graph.edgelist";
        config.grammarFilepath = "path/to/grammar.cfg";

        // Optional parameters
        config.executionMode = "parallel"; // or "serial"
        config.traversalDirection = "fw"; // or "bw" or "bi"
        config.processingStrategy = "gram-driven"; // or "topo-driven"
        config.numThreads = 32; // Number of threads for parallel execution

        // Print the configuration settings
        config.printConfigs();
        // Create a Solver instance with the configuration
        gracfl::Solver* solver = new gracfl::Solver(config);
        // Execute the CFL-reachability analysis
        solver->solve();
        // Retrieve the CFL-reachability graph:
        // outputCFLGraph[node][label] is the set of all reachable destination node IDs from the source node with the edge label.
        std::vector<std::vector<std::unordered_set<ull>>> outputCFLGraph = solver->getGraph();
        // Clean up the solver instance
        delete solver;
    } catch (const std::exception& e) {
        // Handle exceptions and print error messages
        std::cerr << "Error: " << e.what() << "\n\n";
        // Print usage instructions for the GraCFL API
        gracfl::Config::printUsage("example");
    }
}
