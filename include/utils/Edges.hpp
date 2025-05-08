#pragma once

#include <vector>
#include "Types.hpp"

namespace gracfl {
  /**
   * @brief Represents a directed edge with destination and grammar label.
   */
  struct LbldVtx
  {
    uint vtx;
    uint label;
    LbldVtx() {}
    LbldVtx(uint vtx, uint label)
    {
      this->vtx = vtx;
      this->label = label;
    }
  };
  
  /**
   * @brief TemporalVector struct for edge storage with NEW/OLD sliding pointers.
   */
  struct TemporalVector
  {
    uint OLD_END = 0;
    uint NEW_END = 0;
    std::vector<uint> vertexList;
    TemporalVector() {}
  };

  struct TemporalVectorWithLbldVtx
  {
    uint OLD_END = 0;
    uint NEW_END = 0;
    std::vector<LbldVtx> vertexList;
    TemporalVectorWithLbldVtx() {}
  };

  /**
   * @brief Represents a directed edge with source, destination and grammar label.
   */
  struct Edge
  {
    uint from;
    uint to;
    uint label;
    Edge() {}
    Edge(uint from, uint to, uint label)
    {
      this->from = from;
      this->to = to;
      this->label = label;
    }
  };
}