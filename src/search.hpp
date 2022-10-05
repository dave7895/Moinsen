#ifndef SEARCH_HPP
#define SEARCH_HPP
#include "commontypes.hpp"
#include "eval.hpp"
#include "info.hpp"
#include <atomic>
#include <chrono>
#include <libchess/position.hpp>
#include <limits>

namespace search {
int negamax(libchess::Position &pos, int depth, const unsigned int ply,
            std::vector<libchess::Move> &returnMove, std::atomic_bool &stop,
            const std::chrono::time_point<std::chrono::system_clock> &stopTime,
            info::searchInfo &sInfo,
            std::vector<std::vector<libchess::Move>> &moveStack,
            const Options opts = Options(0));
// TODO add int by ref to access eval
libchess::Move iterative_deepening(libchess::Position &pos,
                                   const std::chrono::milliseconds minTime,
                                   std::atomic_bool &stop,
                                   const Options opts = Options(0));
bool isterminal(libchess::Position pos, const Options opts);
} // namespace search
#endif