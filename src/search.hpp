#ifndef SEARCH_HPP
#define SEARCH_HPP
#include "../libchess/src/libchess/position.hpp"
#include "eval.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <libchess/position.hpp>
#include <limits>

namespace search {
int negamax(
    libchess::Position &pos, int depth, int ply, libchess::Move &returnMove,
    std::atomic_bool &stop,
    const std::chrono::time_point<std::chrono::system_clock> &stopTime);
libchess::Move iterative_deepening(
    libchess::Position &pos,
    const std::chrono::time_point<std::chrono::system_clock> stopTime,
    std::atomic_bool &stop);
} // namespace search
#endif