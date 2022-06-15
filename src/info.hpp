#pragma once
#ifndef INFO_HPP
#define INFO_HPP
#include <iostream>
#include <libchess/move.hpp>
namespace info {
struct searchInfo {
  int seldepth;
  long nodecount;

  searchInfo(int seldep = 0, long nodec = 0)
      : seldepth(seldep), nodecount(nodec){};
};

void infopr(int depth, int score, libchess::Move top_move, long time,
            const searchInfo &sInfo);
} // namespace info
#endif