#include <iostream>
#include <chrono>     
#include <stdexcept>  
#include <string> 
#include "solvers/Solver.hpp"

namespace gracfl
{
    Solver::Solver(Config& config)
    : config_(config)
    , grammar_(new Grammar(config.grammarFilepath))  
    {
       solver_ = selectSolver();
       if (solver_ == nullptr)
       {
            throw std::runtime_error("Invalid Config!");
       }
    }

    Solver::~Solver()
    {
        delete grammar_;
    }

    SolverBase* Solver::selectSolver() 
    {
        if (config_.executionMode == "serial") {
            if (config_.traversalDirection == "fw") {
                if (config_.processingStrategy == "gram-driven") {
                    return new SolverFWGram(config_.graphFilepath, *grammar_);
                } else if (config_.processingStrategy == "topo-driven") {
                    return new SolverFWTopo(config_.graphFilepath, *grammar_);
                }
            }
            else if (config_.traversalDirection == "bw") {
                if (config_.processingStrategy == "gram-driven") {
                    return new SolverBWGram(config_.graphFilepath, *grammar_);
                } else if (config_.processingStrategy == "topo-driven") {
                    return new SolverBWTopo(config_.graphFilepath, *grammar_);
                }
            }
            else if (config_.traversalDirection == "bi") {
                if (config_.processingStrategy == "gram-driven") {
                    return new SolverBIGram(config_.graphFilepath, *grammar_);
                } else if (config_.processingStrategy == "topo-driven") {
                    return new SolverBITopo(config_.graphFilepath, *grammar_);
                }
            }                        
        } 
        else if (config_.executionMode == "parallel") {
            if (config_.traversalDirection == "fw") {
                if (config_.processingStrategy == "gram-driven") {
                    return new SolverFWGramParallel(config_.graphFilepath, *grammar_, config_.numThreads);
                } else if (config_.processingStrategy == "topo-driven") {
                    return new SolverFWTopoParallel(config_.graphFilepath, *grammar_, config_.numThreads);
                }
            }
            else if (config_.traversalDirection == "bw") {
                if (config_.processingStrategy == "gram-driven") {
                    return new SolverBWGramParallel(config_.graphFilepath, *grammar_, config_.numThreads);
                } else if (config_.processingStrategy == "topo-driven") {
                    return new SolverBWTopoParallel(config_.graphFilepath, *grammar_, config_.numThreads);
                }
            }
        }
        return nullptr;
    }

    void Solver::solve()
    {
        ull initEdgeCnt = solver_->getEdgeCount();
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Start of the CFL Reachability Analysis" << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        std::chrono::time_point<std::chrono::steady_clock> start, finish;
        start = std::chrono::steady_clock::now();

        solver_->runCFL();

        finish = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds = finish - start;

        std::cout << "---------------------------------------" << std::endl;
        std::cout << "End of the CFL Reachability Analysis" << std::endl;
        std::cout << "---------------------------------------" << std::endl;

        ull newEdgeCnt = solver_->getEdgeCount() - initEdgeCnt;

        std::cout << "---------------Results-----------------" << std::endl;
        std::cout << "---------------------------------------" << std::endl;

        std::cout << "Initial Edges\t= " << initEdgeCnt << std::endl;
        std::cout << "New Edges\t= " << newEdgeCnt << std::endl;
        std::cout << "Total Time\t= " << elapsedSeconds.count() << " seconds" << std::endl;

        std::cout << "---------------END---------------------\n\n\n" << std::endl;
    }

    std::vector<std::vector<std::unordered_set<ull>>> Solver::getGraph()
    {
        return solver_->getGraph();
    }
}