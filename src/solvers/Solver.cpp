#include <iostream>
#include <chrono>     
#include <stdexcept>  
#include <string> 
#include "solvers/Solver.hpp"

namespace gracfl
{
    Solver::Solver(Config& config)
    : config_(config)
    , grammar_(new Grammar(config.grammarFilepath_))  
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
        if (config_.model_ == "gracfl") {
            if (config_.mode_ == "serial") {
                if (config_.direct_ == "fw") {
                    if (config_.strategy_ == "gram-driven") {
                        return new SolverFWGram(config_.graphFilepath_, *grammar_);
                    } else if (config_.strategy_ == "topo-driven") {
                        return new SolverFWTopo(config_.graphFilepath_, *grammar_);
                    }
                }
                else if (config_.direct_ == "bw") {
                    if (config_.strategy_ == "gram-driven") {
                        return new SolverBWGram(config_.graphFilepath_, *grammar_);
                    } else if (config_.strategy_ == "topo-driven") {
                        return new SolverBWTopo(config_.graphFilepath_, *grammar_);
                    }
                }
                else if (config_.direct_ == "bi") {
                    if (config_.strategy_ == "gram-driven") {
                        return new SolverBIGram(config_.graphFilepath_, *grammar_);
                    } else if (config_.strategy_ == "topo-driven") {
                        return new SolverBITopo(config_.graphFilepath_, *grammar_);
                    }
                }                        
            } 
            else if (config_.mode_ == "parallel") {
                if (config_.direct_ == "fw") {
                    if (config_.strategy_ == "gram-driven") {
                        return new SolverFWGramParallel(config_.graphFilepath_, *grammar_, config_.threads_);
                    } else if (config_.strategy_ == "topo-driven") {
                        return new SolverFWTopoParallel(config_.graphFilepath_, *grammar_, config_.threads_);
                    }
                }
                else if (config_.direct_ == "bw") {
                    if (config_.strategy_ == "gram-driven") {
                        return new SolverBWGramParallel(config_.graphFilepath_, *grammar_, config_.threads_);
                    } else if (config_.strategy_ == "topo-driven") {
                        return new SolverBWTopoParallel(config_.graphFilepath_, *grammar_, config_.threads_);
                    }
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
}