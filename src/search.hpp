#ifndef SEARCH_HPP
#define SEARCH_HPP
#include "eval.hpp"
#include "info.hpp"
#include <atomic>
#include <chrono>
#include <libchess/position.hpp>
#include <limits>

namespace search {
int negamax(libchess::Position &pos, int depth, int ply,
            libchess::Move &returnMove, std::atomic_bool &stop,
            const std::chrono::time_point<std::chrono::system_clock> &stopTime, info::searchInfo &sInfo);
libchess::Move iterative_deepening(
    libchess::Position &pos,
    const std::chrono::milliseconds minTime,
    std::atomic_bool &stop);
} // namespace search
#endif