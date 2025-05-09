#pragma once

#include <string>

namespace gracfl 
{
    /**
     * @class Config
     * @brief Manages command-line argument parsing, optional file-based defaults,
     *        and stores solver settings.
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
        /// Derivation strategy: "gram-driven" or "topo-driven".
        std::string processingStrategy;
        /// Number of threads for "parallel" mode.
        unsigned   numThreads;

        Config() = default;
        Config(const std::string& filename);

        /// Load default values from a file (key=value per line).
        void loadFromFile(const std::string& filename);

        /// Parse command-line args, overriding any file-based defaults.
        void parseArgs(int argc, char* argv[]);

        static void printUsage(const char* prog);
        void printConfigs() const;
    };
}