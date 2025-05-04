// std::unique_ptr<GraphBaseBase> graph;
// if (userWants2D) {
//   graph.reset(new Graph2D(...));
// } else {
//   graph.reset(new Graph3D(...));
// }

// graph->loadGraphFile(path,grammar);
// graph->solveCFL(grammar);
// auto& out = graph->getOutEdges();    // same signature, dispatches correctly
