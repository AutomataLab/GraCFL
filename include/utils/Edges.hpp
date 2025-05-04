#pragma once

#include <vector>
#include "Types.hpp"

namespace gracfl {
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

  /**
   * @brief Represents a directed edge with destination and grammar label.
   */
  struct OutEdge
  {
    uint end;
    uint label;
    OutEdge() {}
    OutEdge(uint end, uint label)
    {
      this->end = end;
      this->label = label;
    }
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