#include "utils/Config.hpp"
#include <string>              
#include <unordered_map>       
#include <stdexcept>           
#include <iostream> 
#include <omp.h>

namespace gracfl
{
    Config::Config(int argc, char* argv[])
    {
        parseArgs(argc, argv);
    }
    
    void Config::parseArgs(int argc, char* argv[]) 
    {
        // 1) build flag→value map
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
            return it==kv.end() ? std::string() : it->second;
        };

        // required file paths: graph and grammar (highest priority)
        graphFilepath_ = get("--graph");
        if (graphFilepath_.empty()) {
            throw std::runtime_error("--graph is required");
        }

        grammarFilepath_ = get("--grammar");
        if (grammarFilepath_.empty()) {
            throw std::runtime_error("--grammar is required");
        }
    
        // model
        model_ = get("--model");
        if (model_.empty()) {
            model_ = "gracfl";
        } else if (model_ != "gracfl" && model_ != "base") {
            throw std::runtime_error("Invalid --model '" + model_ + "'");
        }

        // mode
        mode_ = get("--mode");
        if (mode_.empty()) {
            mode_ = "serial";
        } else if (mode_ != "serial" && mode_ != "parallel") {
            throw std::runtime_error(
              "Invalid --mode '" + mode_ + "'. Allowed: serial, parallel");
        }
    
        // direct
        direct_ = get("--direct");
        if (direct_.empty()) {
            // default unless base
            if (model_ == "base")
                throw std::runtime_error(
                  "--direct is required when model='base'");
            if (mode_ == "parallel") {
                direct_ = "fw";
            } else {
                direct_ = "bi";
            }
        }
        if (direct_ != "fw" && direct_ != "bw" && direct_ != "bi") {
            throw std::runtime_error(
              "Invalid --direct '" + direct_ + "'. Allowed: fw, bw, bi");
        }
    
        // ** strategy **
        strategy_ = get("--strategy");
        if (strategy_.empty()) {
            strategy_ = "gram-driven";
        } else if (strategy_ != "gram-driven" && strategy_ != "topo-driven") {
            throw std::runtime_error(
                "Invalid --strategy '" + strategy_ + "'. Allowed: gram-driven, topo-driven");
        }
        
        // threads
        threads_ = 1;
        if (mode_ == "parallel") {
            auto ts = get("--thread");
            if (ts.empty()) {
                // default to system concurrency
                // unsigned hc = std::thread::hardware_concurrency();
                // use OpenMP's default thread count
                int omp_thr = omp_get_max_threads();
                threads_ = (omp_thr>0 ? omp_thr : 1);
            } else {
                try {
                    int x = std::stoi(ts);
                    if (x <= 0) throw std::invalid_argument("<=0");
                    threads_ = unsigned(x);
                } catch (...) {
                    throw std::runtime_error(
                      "Invalid --thread '" + ts + "'. Must be >0");
                }
            }
        }
    }

    void Config::printUsage(const char* prog) {
        std::cerr
          << "Usage:\n"
          << "  " << prog
          << " --graph <graphfile> --grammar <grammarfile>\n"
          << "        [--model <gracfl|base>]\n"
          << "        [--direct <fw|bw|bi>]\n"
          << "        [--mode <serial|parallel>]\n"
          << "        [--thread <n>]\n\n"
          << "Required:\n"
          << "  --graph    Path to the input graph file\n"
          << "  --grammar  Path to the grammar file\n\n"
          << "Defaults (when optional flags omitted):\n"
          << "  model   = gracfl\n"
          << "  direct  = bi\n"
          << "  mode    = serial\n"
          << "  threads = 1 (or max available hardware threads if mode=parallel and --thread not set)\n\n"
          << "Rules:\n"
          << "  • --graph and --grammar are mandatory\n"
          << "  • if model==base, must supply --direct\n"
          << "  • direct defaults to bi otherwise (validate fw|bw|bi)\n"
          << "  • mode defaults to serial (validate serial|parallel)\n"
          << "  • if mode==parallel & no --thread, use max available hardware threads\n";
    }

    void Config::printConfigs() {
        // print all the settings up front
        std::cout << "Configuration:\n"
                  << "  graph   = " << graphFilepath_ << "\n"
                  << "  grammar = " << grammarFilepath_ << "\n"
                  << "  model   = " << model_ << "\n"
                  << "  direct  = " << direct_ << "\n"
                  << "  mode    = " << mode_ << "\n";
        if (mode_ == "parallel")
            std::cout << "  threads = " << threads_ << "\n";
        std::cout << std::endl;
    }
}