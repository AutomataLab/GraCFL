#include <iostream>
#include <chrono>     
#include <stdexcept>  
#include <string> 
#include "gracfl/GraCFLSolver.hpp"

namespace gracfl
{
    GraCFLSolver::GraCFLSolver(Config& config)
    {
        config_ = config;
        // printConfigs();
        grammar_ = new Grammar(config_.grammarfile_);
        // the graph processing depends on the grammar_, so process the grammar_ first
        graph_ = processGraph();
        if (graph_ == nullptr)
        {
            throw std::runtime_error("Invalid graph!");
        }
    }

    GraCFLSolver::~GraCFLSolver()
    {
        delete graph_;
        graph_ = nullptr;
        delete grammar_;
        grammar_ = nullptr;
    }

    Graph* GraCFLSolver::processGraph() 
    {
        if (config_.model_ == "gracfl") {
            if (config_.mode_ == "serial") {
                if (config_.direct_ == "fw") {
                    FWGracfl* graph = new FWGracfl(config_, *grammar_);
                    return graph;
                }
                else if (config_.direct_ == "bw") {
                    BWGracfl* graph = new BWGracfl(config_, *grammar_);
                    return graph;
                }
                else if (config_.direct_ == "bi") {
                    BIGracfl* graph = new BIGracfl(config_, *grammar_);
                    return graph;
                }                        
            } 
            else if (config_.mode_ == "parallel") {
                if (config_.direct_ == "fw") {
                    FWGracflParallel* graph = new FWGracflParallel(config_, *grammar_);
                    return graph;
                }
            }
            // else { // parallel
            //     if (config_.direct_ == "fw") 
            //     {     
            //         // run_gracfl_fw_parallel();
            //     }
            //     else if (config_.direct_ == "bw") 
            //     {
            //         // run_gracfl_bw_parallel();
            //     }
            //     else if (config_.direct_ == "bi") 
            //     {                        
            //         // run_gracfl_bi_parallel();
            //     }
            // }
        } 
        // else { // model == "base"
        //     if (config_.mode_ == "serial") {
        //         if (config_.direct_ == "fw")      
        //             run_base_fw_serial();
        //         else if (config_.direct_ == "bw") 
        //             run_base_bw_serial();
        //         else                          
        //             run_base_bi_serial();
        //     } else { // parallel
        //         if (config_.direct_ == "fw")      
        //             run_base_fw_parallel(config_.threads_);
        //         else if (config_.direct == "bw") 
        //             run_base_bw_parallel(config_.threads_);
        //         else                          
        //             run_base_bi_parallel(config_.threads_);
        //     }
        // }

        return nullptr;
    }

    void GraCFLSolver::solveCFL()
    {
        ull initEdgeCnt = graph_->countEdge();

        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Start of the CFL Reachability Analysis" << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        std::chrono::time_point<std::chrono::steady_clock> start, finish;
        start = std::chrono::steady_clock::now();

        graph_->solve(*grammar_);

        finish = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds = finish - start;

        std::cout << "---------------------------------------" << std::endl;
        std::cout << "End of the CFL Reachability Analysis" << std::endl;
        std::cout << "---------------------------------------" << std::endl;

        ull newEdgeCnt = graph_->countEdge() - initEdgeCnt;

        std::cout << "---------------Results-----------------" << std::endl;
        std::cout << "---------------------------------------" << std::endl;

        std::cout << "Initial Edges\t= " << initEdgeCnt << std::endl;
        std::cout << "New Edges\t= " << newEdgeCnt << std::endl;
        std::cout << "Total Time\t= " << elapsedSeconds.count() << " seconds" << std::endl;

        std::cout << "---------------END---------------------\n\n\n" << std::endl;
    }
}