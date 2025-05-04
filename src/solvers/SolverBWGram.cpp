// #include <iostream>
// #include "solvers/SolverBWGram.hpp"

// namespace gracfl 
// {
//     SolverBWGram::SolverBWGram(Grammar* grammar, std::unique_ptr<Graph> graph)
//     {
//         grammar_ = grammar;
//         graph_ = std::move(graph);
//     }

//     void  SolverBWGram::runCFL()
//     {
//         addAllSelfEdges(grammar); // add epsilon edges
//         uint itr = 0;
//         bool terminate;
//         do {
//             itr++;
//             terminate = true;
//             singleIteration(grammar, terminate);
//             std::cout << "Iteration " << itr << std::endl;
//         } while(!terminate);
//     }

//     void  SolverBWGram::runSingleIteration(bool& terminate)
//     {
//         for (uint g = 0; g < grammar.getLabelSize(); g++)
//         {
//             for (int i = 0; i < getNodeSize(); i++)
//             {
//                 uint inNbr1;
//                 uint START_NEW = inEdges_[g][i].OLD_END;
//                 uint END_NEW = inEdges_[g][i].NEW_END;

//                 for (uint j = START_NEW; j < END_NEW; j++)
//                 {
//                     inNbr1 = inEdges_[g][i].vertexList[j];
                    
//                     for (uint m = 0; m < grammar.rule2Index(g).size(); m++)
//                     {
//                         uint A = grammar.rule2Index(g)[m];
//                         EdgeForReading newEdge(inNbr1, i, A);
//                         addEdge(newEdge, terminate);
//                     }

//                     for (uint m = 0; m < grammar.rule3RightIndex(g).size(); m++)
//                     {
//                         uint B = grammar.rule3RightIndex(g)[m].first;
//                         uint A = grammar.rule3RightIndex(g)[m].second;

//                         uint START_OLD_OUT = 0;
//                         uint END_NEW_OUT = inEdges_[B][inNbr1].NEW_END;
//                         for (uint h = START_OLD_OUT; h < END_NEW_OUT; h++)
//                         {
//                             uint inNbr2 = inEdges_[B][inNbr1].vertexList[h];
//                             EdgeForReading newEdge(inNbr2, i, A);
//                             addEdge(newEdge, terminate);
//                         }
//                     }
//                 }

//                 uint START_OLD = 0;
//                 uint END_OLD = inEdges_[g][i].OLD_END;
//                 for (uint j = START_OLD; j < END_OLD; j++)
//                 {
//                     inNbr1 = inEdges_[g][i].vertexList[j];

//                     for (uint m = 0; m < grammar.rule3RightIndex(g).size(); m++)
//                     {
//                         // C = g
//                         uint B = grammar.rule3RightIndex(g)[m].first;
//                         uint A = grammar.rule3RightIndex(g)[m].second;

//                         uint START_NEW_OUT = inEdges_[B][inNbr1].OLD_END;
//                         uint END_NEW_OUT = inEdges_[B][inNbr1].NEW_END;

//                         for (uint h = START_NEW_OUT; h < END_NEW_OUT; h++)
//                         {
//                             uint inNbr2 = inEdges_[B][inNbr1].vertexList[h];
//                             EdgeForReading newEdge(inNbr2, i, A);
//                             addEdge(newEdge, terminate);
//                         }
//                     }
//                 }
//             }
//         }

//         // ----------------- Update Sliding Pointers -----------------
//         for (uint g = 0; g < grammar.getLabelSize(); g++)
//         {
//             for (int i = 0; i < getNodeSize(); i++)
//             {
//                 inEdges_[g][i].OLD_END = inEdges_[g][i].NEW_END;
//                 inEdges_[g][i].NEW_END = inEdges_[g][i].vertexList.size();
//             }
//         }
//     }

    

//     void SolverBWGram::addSelfEdges()
//     {
//         for (int l = 0; l < grammar.getRule1().size(); l++)
//         {
//             for (int i = 0; i < getNodeSize(); i++)
//             {
//                 EdgeForReading edge(i, i, grammar.getRule1()[l][0]);
//                 addSelfEdge(edge);
//             }
//         }
//     }
// }