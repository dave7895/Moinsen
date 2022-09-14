#ifndef EVALUATION_HPP
#define EVALUATION_HPP
#include <libchess/position.hpp>
#include "pst.hpp"
#include "commontypes.hpp"
namespace evaluation {
int evaluate(const libchess::Position &pos, const int ply = 1, Options opts=Options(1));
std::pair<uint8_t, uint8_t> count_checks(libchess::Position pos, const Options &opts);
const int mate_score = 15000;
constexpr int mate_threshhold = 14000;
const int contempt = 0;
const std::vector<int> pieceToVal = {100, 310, 330, 500, 900, 0};
constexpr libchess::Bitboard winKOTH =
    (libchess::bitboards::FileD | libchess::bitboards::FileE) &
    (libchess::bitboards::Rank4 | libchess::bitboards::Rank5);

} // namespace evaluation
#endif