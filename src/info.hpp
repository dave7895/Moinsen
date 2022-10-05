#pragma once
#ifndef INFO_HPP
#define INFO_HPP
#include "eval.hpp"
#include <iostream>
#include <libchess/move.hpp>
namespace info {
struct searchInfo {
  unsigned int seldepth;
  long nodecount;

  searchInfo(int seldep = 0, long nodec = 0)
      : seldepth(seldep), nodecount(nodec){};
};

void infopr(const int depth, const int score,
            const std::vector<libchess::Move> &top_move, const long time,
            const searchInfo &sInfo);
} // namespace info
#endif