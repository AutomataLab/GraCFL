#include "utils/Config.hpp"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <omp.h>


// # Config – one key = value per line
// # Required inputs:
// graphFilePath    = /home/user/data/graph.edgelist
// grammarFilePath  = /home/user/data/grammar.cfg

// # Optional settings (defaults shown):
// executionMode     = parallel          # serial or parallel
// traversalDirection   = fw                # fw, bw, or bi
// processingStrategy = topo-driven       # gram-driven or topo-driven
// numThreads  = 8                 # positive integer, only used in parallel mode


namespace gracfl {

// helper to trim both ends in-place
static void trim(std::string& s) {
    auto not_space = [](int ch){ return !std::isspace(ch); };
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(), not_space));
    s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(),
            s.end());
}

Config::Config(const std::string& filename) {
    loadFromFile(filename);
}


void Config::loadFromFile(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open config file: " + filename);
    }

    // no user value → default to OpenMP max
    int omp_thr = omp_get_max_threads();
    numThreads = (omp_thr > 0 ? omp_thr : 1);

    std::string line;
    while (std::getline(ifs, line)) {
        trim(line);
        if (line.empty() || line[0] == '#')
            continue;

        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);
        trim(key);
        trim(val);

        if (key == "graphFilepath") {
            graphFilepath = val;
        }
        else if (key == "grammarFilepath") {
            grammarFilepath = val;
        }
        else if (key == "executionMode") {
            executionMode = val;
        }
        else if (key == "traversalDirection") {
            traversalDirection = val;
        }
        else if (key == "processingStrategy") {
            processingStrategy = val;
        }
        else if (key == "numThreads") {
            try {
                numThreads = static_cast<unsigned>(std::stoul(val));
            } catch (...) {
                throw std::runtime_error("Invalid numThreads value: " + val);
            }
        }
        else {
            throw std::runtime_error("Unknown config key: " + key);
        }
    }

    // validate required fields
    if (graphFilepath.empty())
        throw std::runtime_error("Config file must specify 'graphFilepath = <path>'");
    if (grammarFilepath.empty())
        throw std::runtime_error("Config file must specify 'grammarFilepath = <path>'");

    // apply defaults & validate
    if (executionMode.empty())
        executionMode = "serial";
    else if (executionMode != "serial" && executionMode != "parallel")
        throw std::runtime_error("executionMode must be 'serial' or 'parallel'");

    if (traversalDirection.empty())
        traversalDirection = (executionMode == "parallel" ? "fw" : "bi");
    else if (traversalDirection != "fw" &&
             traversalDirection != "bw" &&
             traversalDirection != "bi")
    {
        throw std::runtime_error("traversalDirection must be 'fw', 'bw', or 'bi'");
    }

    if (processingStrategy.empty())
        processingStrategy = "gram-driven";
    else if (processingStrategy != "gram-driven" &&
             processingStrategy != "topo-driven")
    {
        throw std::runtime_error("processingStrategy must be 'gram-driven' or 'topo-driven'");
    }

    if (executionMode == "parallel" && numThreads <= 0) {
        throw std::runtime_error("numThreads must be a positive integer");
    }
}

void Config::parseArgs(int argc, char* argv[]) {
    std::unordered_map<std::string, std::string> kv;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a.rfind("--", 0) != 0)
            throw std::runtime_error("Unknown argument: " + a);

        std::string val;
        if (i+1 < argc && std::string(argv[i+1]).rfind("--",0) != 0)
            val = argv[++i];
        kv[a] = val;
    }
    auto get = [&](const char* key){
        auto it = kv.find(key);
        return it == kv.end() ? std::string() : it->second;
    };

    // required file paths
    {
        auto v = get("--graphFilepath");
        if (!v.empty()) graphFilepath = v;
        if (graphFilepath.empty())
            throw std::runtime_error("--graphFilepath is required");
    }
    {
        auto v = get("--grammarFilepath");
        if (!v.empty()) grammarFilepath = v;
        if (grammarFilepath.empty())
            throw std::runtime_error("--grammarFilepath is required");
    }

    // execution mode
    {
        auto v = get("--executionMode");
        if (!v.empty()) executionMode = v;
        if (executionMode.empty()) executionMode = "serial";
        else if (executionMode!="serial" && executionMode!="parallel")
            throw std::runtime_error(
              "Invalid --executionMode '" + executionMode + "'. Allowed: serial, parallel");
    }

    // traversal direction
    {
        auto v = get("--traversalDirection");
        if (!v.empty()) traversalDirection = v;
        if (traversalDirection.empty())
            traversalDirection = (executionMode=="parallel" ? "fw" : "bi");
        if (traversalDirection!="fw" &&
            traversalDirection!="bw" &&
            traversalDirection!="bi")
        {
            throw std::runtime_error(
              "Invalid --traversalDirection '" + traversalDirection + 
              "'. Allowed: fw, bw, bi");
        }
    }

    // derivation strategy
    {
        auto v = get("--processingStrategy");
        if (!v.empty()) processingStrategy = v;
        if (processingStrategy.empty()) processingStrategy = "gram-driven";
        else if (processingStrategy!="gram-driven" &&
                 processingStrategy!="topo-driven")
        {
            throw std::runtime_error(
              "Invalid --processingStrategy '" + processingStrategy +
              "'. Allowed: gram-driven, topo-driven");
        }
    }

    // threads
    if (executionMode == "parallel") {
        auto ts = get("--numThreads");
        if (!ts.empty()) {
            try {
                int x = std::stoi(ts);
                if (x <= 0) throw 0;
                numThreads = static_cast<unsigned>(x);
            } catch (...) {
                throw std::runtime_error(
                  "Invalid --numThreads '" + ts + "'. Must be >0");
            }
        } else {
            int omp_thr = omp_get_max_threads();
            numThreads = (omp_thr > 0 ? omp_thr : 1);
        }
    }
}

void Config::printUsage(const char* prog) {
    std::cerr
      << "This program reads all settings from a plain-text file (default name: 'Config').\n"
      << "The file must contain one key = value per line. Supported keys (must match Config class fields):\n"
      << "  graphFilepath      = <path to graph file>            (required)\n"
      << "  grammarFilepath    = <path to grammar file>          (required)\n"
      << "  executionMode      = serial | parallel               (default: serial)\n"
      << "  traversalDirection = fw | bw | bi                    (default: bi, or fw if executionMode=parallel)\n"
      << "  processingStrategy = gram-driven | topo-driven       (default: gram-driven)\n"
      << "  numThreads         = <positive integer>              (default: 1, or max threads if parallel)\n\n"
      << "\n"
      << "----------------------------------------------\n"
      << "Example 'Config' file:\n"
      << "----------------------------------------------\n"
      << "# solver configuration using class field names\n"
      << "graphFilepath      = /home/user/data/graph.edgelist\n"
      << "grammarFilepath    = /home/user/data/grammar.cfg\n"
      << "executionMode      = parallel\n"
      << "traversalDirection = fw\n"
      << "processingStrategy = gram-driven\n"
      << "numThreads         = 32\n"
      << "----------------------------------------------\n";
}


void Config::printConfigs() const {
    std::cout << "Configuration:\n"
              << "  graphFilepath       = " << graphFilepath      << "\n"
              << "  grammarFilepath     = " << grammarFilepath    << "\n"
              << "  executionMode       = " << executionMode      << "\n"
              << "  traversalDirection  = " << traversalDirection << "\n"
              << "  processingStrategy  = " << processingStrategy << "\n";
    if (executionMode == "parallel")
        std::cout << "  numThreads          = " << numThreads << "\n";
    std::cout << std::endl;
}

} // namespace gracfl
