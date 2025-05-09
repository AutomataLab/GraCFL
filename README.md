# GraCFL

## Table of Contents
- [Project Overview](#project-overview)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Build Instructions](#build-instructions)
- [Running the CFL Reachability Analysis](#running-the-cfl-reachability-analysis)
- [Example Graph and Grammar](#example-graph-and-grammar)
- [GraCFL as a Library](#gracfl-as-a-library)

## Project Overview
**GraCFL** is designed for high-performance CFL reachability analysis. The project includes various models for performing CFL reachability computations with different optimizations and parallel strategies.

## Dependencies
The project depends on the following libraries and tools:

- **CMake** (minimum version 3.26)
- **C++11 or higher** (supported by the project)
- **TBB** (Threading Building Blocks)
- **jemalloc** (for memory allocation)
- **OpenMP** (optional for parallelism)

The project automatically fetches and builds the necessary dependencies if they are not found on the system.

## Installation

### Clone the Repository
To get started, clone the repository using the following command:

```bash
git clone https://github.com/anonym117/GraCFL.git
cd GraCFL
```

## Build Instructions

To build the project, follow these steps:

1. **Create a build directory:**

    First, create a directory for building the project and move into that directory:

    ```bash
    mkdir build
    cd build
    ```

2. **Configure the project using CMake:**

    Run the `cmake` command from the `build` directory to configure the project. By default, the project is set to use Release mode with `-O3` optimization:

    ```bash
    cmake ..
    ```
3. **Build the project:**

    Run the `make` command to build the project:
   
    ```bash
    make
    ```
    Executable named `gracfl` will be placed in build/bin/

## Running the CFL Reachability Analysis
After building, you can run the generated executables from the `build/bin/` directory as follows:

### First Create your ConfigGraCFL file

Place a plain-text file named  `ConfigGraCFL` (no extension) alongside the executable (i.e. in `build/bin/`) with one `key = value` per line:

```text
# Required inputs:
graphFilepath    = /home/user/data/graph.txt
grammarFilepath  = /home/user/data/grammar.txt

# Optional settings (defaults shown):
executionMode      = serial           # serial or parallel (default: serial)
traversalDirection = bi               # fw, bw, or bi (default: bi if serial or fw if parallel)
processingStrategy = gram-driven      # gram-driven or topo-driven (default: gram-driven)
numThreads         = 32               # positive integer, only used if parallel (default: all available cores)
```

### Then Run the Following Command

```bash
./gracfl
```

Note: The `ConfigGraCFL` file and the executable `gracfl` need to be in the same directory.

## Example Graph and Grammar

To help you get started, here’s a minimal example of an input graph and a normalized (Chomsky Normal Form) grammar for CFL reachability.

### Sample Graph (`sample_graph.txt`)

Each line is a directed, labeled edge in the format: `src dst label`
```text
0 1 a
1 2 b
2 3 a
3 4 b
```

### Sample Normalized Grammar (sample_grammar.txt)
Each line represents a production rule in the grammar. A rule of type `A = BC` is written in this format `A    B    C`
```
A                   # epsilon rule creates self edges
A    a              # unary rule
B    b              # unary rule
S    A    B         # binary rule 
```

## GraCFL as a Library
1. Clone into your project
    In your external project’s root directory:
    ```text
    https://github.com/AutomataLab/GraCFL.git
    ```
2. CMake configuration
    In your `CMakeLists.txt`, add GraCFL as a subdirectory and link against its static library:
    ```cmake
    # Add the GraCFL library as a subdirectory
    add_subdirectory(GraCFL)
    ```
    ```cmake
    # Link against the static library
    target_link_libraries(run_graCFL PRIVATE graCFLlib)
    ```

    An Example `CMakeLists.txt` of an external project that uses GraCFL as a library
    ```cmake
    cmake_minimum_required(VERSION 3.15)
    project(ExternalGraCFLUser)
    
    # Add the GraCFL library as a subdirectory
    add_subdirectory(GraCFL)
  
    # Other configurations in your CMakeLists.txt
     
    # Build your executable
    add_executable(ExternalGraCFLUserExecutable main.cpp)
    
    # Link against the static library
    target_link_libraries(run_graCFL PRIVATE graCFLlib)
    ```
    
3. Sample usage in code

    ```cpp
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
    ```


