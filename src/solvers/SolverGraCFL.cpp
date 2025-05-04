#include <iostream>
#include <chrono>     
#include <stdexcept>  
#include <string> 
#include "solvers/SolverGraCFL.hpp"

Config config = Config();
gracfl:: SolverGraCFL solver = SolverGracfl(config);
solver.solve();
solver.getGraph();


namespace gracfl
{
    SolverGraCFL::SolverGraCFL(int argc, char* argv[])
    : config_(argc, argv),
      grammar_(config_.grammarfile_)
    {
        Graph* graph_ = X;
        solver = new
    }

    SolverGraCFL::~SolverGraCFL()
    {
        
    }

    // std::unique_ptr<Graph> SolverGraCFL::selectGraph() 
    // {
    //     if (config_.model_ == "gracfl") {
    //         if (config_.mode_ == "serial") {
    //             if (config_.direct_ == "fw") {
    //                 return std::make_unique<Graph3DOut>(config_, *grammar_);
    //             }
    //             else if (config_.direct_ == "bw") {
    //                 return std::make_unique<Graph3DIn>(config_, *grammar_);
    //             }
    //             else if (config_.direct_ == "bi") {
    //                 return std::make_unique<Graph3DBi>(config_, *grammar_);
    //             }                        
    //         } 
    //         else if (config_.mode_ == "parallel") {
    //             if (config_.direct_ == "fw") {
    //                 return std::make_unique<Graph3DOut>(config_, *grammar_);
    //             }
    //         }
    //     } 
    //     return nullptr;
    // }

    // SolverGraCFL* SolverGraCFL::selectSolver() 
    // {
    //     if (config_.model_ == "gracfl") {
    //         if (config_.mode_ == "serial") {
    //             if (config_.direct_ == "fw") {
    //                 return new SolverFWGram(config_, *grammar_);
    //             }
    //             else if (config_.direct_ == "bw") {
    //                 return new SolverBWGram(config_, *grammar_);
    //             }
    //             else if (config_.direct_ == "bi") {
    //                 return new SolverBIGram(config_, *grammar_);
    //             }                        
    //         } 
    //         else if (config_.mode_ == "parallel") {
    //             if (config_.direct_ == "fw") {
    //                 return new SolverFWGramParallel(config_, *grammar_);
    //             }
    //         }
    //         // else { // parallel
    //         //     if (config_.direct_ == "fw") 
    //         //     {     
    //         //         // run_gracfl_fw_parallel();
    //         //     }
    //         //     else if (config_.direct_ == "bw") 
    //         //     {
    //         //         // run_gracfl_bw_parallel();
    //         //     }
    //         //     else if (config_.direct_ == "bi") 
    //         //     {                        
    //         //         // run_gracfl_bi_parallel();
    //         //     }
    //         // }
    //     } 
    //     // else { // model == "base"
    //     //     if (config_.mode_ == "serial") {
    //     //         if (config_.direct_ == "fw")      
    //     //             run_base_fw_serial();
    //     //         else if (config_.direct_ == "bw") 
    //     //             run_base_bw_serial();
    //     //         else                          
    //     //             run_base_bi_serial();
    //     //     } else { // parallel
    //     //         if (config_.direct_ == "fw")      
    //     //             run_base_fw_parallel(config_.threads_);
    //     //         else if (config_.direct == "bw") 
    //     //             run_base_bw_parallel(config_.threads_);
    //     //         else                          
    //     //             run_base_bi_parallel(config_.threads_);
    //     //     }
    //     // }

    //     return nullptr;
    // }

    void runSolverFWGram()
    {
        Graph3DOut graph(path, grammar);
        SolverFWGram solver* =  new SolverFWGram(grammar, graph);
        solver.solve
    }

    void SolverGraCFL::solve()
    {
        if (config_.model_ == "gracfl") {
            if (config_.mode_ == "serial") {
                if (config_.direct_ == "fw") {
                    runSolverFWGram();
                }
                else if (config_.direct_ == "bw") {
                    return new SolverBWGram(config_, *grammar_);
                }
                else if (config_.direct_ == "bi") {
                    return new SolverBIGram(config_, *grammar_);
                }                        
            } 
            else if (config_.mode_ == "parallel") {
                if (config_.direct_ == "fw") {
                    return new SolverFWGramParallel(config_, *grammar_);
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