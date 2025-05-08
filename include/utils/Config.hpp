#pragma once

#include <string>

namespace gracfl 
{
    /**
     * @class Config
     * @brief Manages command-line argument parsing and stores solver settings.
     *
     * Parses user-provided flags to determine input file paths, algorithm model,
     * traversal direction, execution mode, and threading parameters.
     */
    class Config {
    public:
        /// Path to the input graph file (required).
        std::string graphFilepath;
        /// Path to the context-free grammar file (required).
        std::string grammarFilepath;
        /// Direction of traversal: "fw", "bw", or "bi".
        std::string traversalDirection;
        /// Execution mode: "serial" or "parallel".
        std::string executionMode;
        std::string derivationStrategy;
         /// Number of threads for "parallel" mode .
        unsigned   numThreads;


        /**
         * @brief Default constructor; leaves configuration uninitialized until parseArgs is called.
         */
        Config() = default; 

        /**
         * @brief Constructs a Config and immediately parses command-line arguments.
         * @param argc Number of command-line arguments.
         * @param argv Array of command-line argument strings.
         * @throws std::invalid_argument if required flags are missing or malformed.
         */
        Config(int argc, char* argv[]);

        /**
         * @brief Parses command-line arguments and populates configuration fields.
         * @param argc Number of command-line arguments.
         * @param argv Array of argument strings.
         * @throws std::invalid_argument if parsing fails or required options are missing.
         */
        void parseArgs(int argc, char* argv[]);

        /**
         * @brief Prints the current configuration to standard output.
         */
        void printConfigs();

        /**
         * @brief Displays usage instructions for the solver.
         * @param prog Name of the executable (e.g., argv[0]).
         */
        static void printUsage(const char* prog);
    };
}
