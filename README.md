# GraCFL

## Table of Contents
- [Project Overview](#project-overview)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Build Instructions](#build-instructions)

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

```
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
