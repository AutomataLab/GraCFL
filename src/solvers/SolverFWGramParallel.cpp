// #include <iostream>
// #include "solvers/SolverFWGramParallel.hpp"

// namespace gracfl 
// {
//     SolverFWGramParallel::SolverFWGramParallel(Grammar* grammar, std::unique_ptr<Graph> graph, uint numOfThreads)
//     : SolverFWGram(grammar, graph)
//     {
//         numOfThreads_ = numOfThreads;
//     }

//     void  SolverFWGramParallel::runCFL()
//     { 
//         addAllSelfEdges(grammar); // add epsilon edges
//         uint itr = 0;
//         bool terminate;
//         uint noOfThreads = getConfig().threads_;
//         do {
//             itr++;
//             terminate = true;
//             singleIterationParallel(grammar, terminate, noOfThreads);
//             std::cout << "Iteration " << itr << std::endl;
//         } while(!terminate);
//     }

//     void  SolverFWGramParallel::runSingleIterationParallelLabel(bool& terminate)
//     {
//         for (uint g = 0; g < grammar.getLabelSize(); g++)
//         {
//             #pragma omp parallel for schedule(static) num_threads(noOfThreads)
//             for (uint i = 0; i < getNodeSize(); i++)
//             {
//                 uint nbr;
//                 uint START_NEW = getOutEdges()[g][i].OLD_END;
//                 uint END_NEW = getOutEdges()[g][i].NEW_END;

//                 for (uint j = START_NEW; j < END_NEW; j++)
//                 {
//                     nbr = getOutEdges()[g][i].vertexList[j];
//                     for (uint m = 0; m < grammar.rule2Index(g).size(); m++)
//                     {
//                         uint A = grammar.rule2Index(g)[m];
//                         EdgeForReading newEdge(i, nbr, A);
//                         addEdge(newEdge, terminate);
//                     }

//                     for (uint m = 0; m < grammar.rule3LeftIndex(g).size(); m++)
//                     {
//                         uint C = grammar.rule3LeftIndex(g)[m].first;
//                         uint A = grammar.rule3LeftIndex(g)[m].second;

//                         uint START_OLD_OUT = 0;
//                         uint END_NEW_OUT = getOutEdges()[C][nbr].NEW_END;
//                         for (uint h = START_OLD_OUT; h < END_NEW_OUT; h++)
//                         {
//                             uint outNbr = getOutEdges()[C][nbr].vertexList[h];
//                             EdgeForReading newEdge(i, outNbr, A);
//                             addEdge(newEdge, terminate);
//                         }
//                     }
//                 }

//                 uint START_OLD = 0;
//                 uint END_OLD = getOutEdges()[g][i].OLD_END;
//                 for (uint j = START_OLD; j < END_OLD; j++)
//                 {
//                     nbr = getOutEdges()[g][i].vertexList[j];
//                     for (uint m = 0; m < grammar.rule3LeftIndex(g).size(); m++)
//                     {
//                         uint C = grammar.rule3LeftIndex(g)[m].first;
//                         uint A = grammar.rule3LeftIndex(g)[m].second;

//                         uint START_NEW_OUT = getOutEdges()[C][nbr].OLD_END;
//                         uint END_NEW_OUT = getOutEdges()[C][nbr].NEW_END;
//                         for (uint h = START_NEW_OUT; h < END_NEW_OUT; h++)
//                         {
//                             uint outNbr = getOutEdges()[C][nbr].vertexList[h];
//                             EdgeForReading newEdge(i, outNbr, A);
//                             addEdge(newEdge, terminate);
//                         }
//                     }
//                 }
//             }
//         }

//         // ----------------- Update Sliding Pointers -----------------
//         for (uint g = 0; g < grammar.getLabelSize(); g++)
//         {
//             #pragma omp parallel for schedule(static) num_threads(noOfThreads)
//             for (int i = 0; i < getNodeSize(); i++)
//             {
//                 getOutEdges()[g][i].OLD_END = getOutEdges()[g][i].NEW_END;
//                 getOutEdges()[g][i].NEW_END = getOutEdges()[g][i].vertexList.size();
//             }
//         }
//     }

//     void  SolverFWGramParallel::runSingleIterationParallel(bool& terminate)
//     {
//         #pragma omp parallel for schedule(static) num_threads(noOfThreads)
//         for (uint i = 0; i < getNodeSize(); i++)
//         {
//             for (uint g = 0; g < grammar.getLabelSize(); g++)
//             {
//                 uint nbr;
//                 uint START_NEW = getOutEdges()[g][i].OLD_END;
//                 uint END_NEW = getOutEdges()[g][i].NEW_END;

//                 for (uint j = START_NEW; j < END_NEW; j++)
//                 {
//                     nbr = getOutEdges()[g][i].vertexList[j];
//                     for (uint m = 0; m < grammar.rule2Index(g).size(); m++)
//                     {
//                         uint A = grammar.rule2Index(g)[m];
//                         EdgeForReading newEdge(i, nbr, A);
//                         addEdge(newEdge, terminate);
//                     }

//                     for (uint m = 0; m < grammar.rule3LeftIndex(g).size(); m++)
//                     {
//                         uint C = grammar.rule3LeftIndex(g)[m].first;
//                         uint A = grammar.rule3LeftIndex(g)[m].second;

//                         uint START_OLD_OUT = 0;
//                         uint END_NEW_OUT = getOutEdges()[C][nbr].NEW_END;
//                         for (uint h = START_OLD_OUT; h < END_NEW_OUT; h++)
//                         {
//                             uint outNbr = getOutEdges()[C][nbr].vertexList[h];
//                             EdgeForReading newEdge(i, outNbr, A);
//                             addEdge(newEdge, terminate);
//                         }
//                     }
//                 }

//                 uint START_OLD = 0;
//                 uint END_OLD = getOutEdges()[g][i].OLD_END;
//                 for (uint j = START_OLD; j < END_OLD; j++)
//                 {
//                     nbr = getOutEdges()[g][i].vertexList[j];
//                     for (uint m = 0; m < grammar.rule3LeftIndex(g).size(); m++)
//                     {
//                         uint C = grammar.rule3LeftIndex(g)[m].first;
//                         uint A = grammar.rule3LeftIndex(g)[m].second;

//                         uint START_NEW_OUT = getOutEdges()[C][nbr].OLD_END;
//                         uint END_NEW_OUT = getOutEdges()[C][nbr].NEW_END;
//                         for (uint h = START_NEW_OUT; h < END_NEW_OUT; h++)
//                         {
//                             uint outNbr = getOutEdges()[C][nbr].vertexList[h];
//                             EdgeForReading newEdge(i, outNbr, A);
//                             addEdge(newEdge, terminate);
//                         }
//                     }
//                 }
//             }
//         }

//         // ----------------- Update Sliding Pointers -----------------
//         #pragma omp parallel for schedule(static) num_threads(noOfThreads)
//         for (int i = 0; i < getNodeSize(); i++)
//         {   
//             for (uint g = 0; g < grammar.getLabelSize(); g++)
//             {
//                 getOutEdges()[g][i].OLD_END = getOutEdges()[g][i].NEW_END;
//                 getOutEdges()[g][i].NEW_END = getOutEdges()[g][i].vertexList.size();
//             }
//         }
//     }
// }