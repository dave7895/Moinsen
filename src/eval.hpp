#ifndef EVALUATION_HPP
#define EVALUATION_HPP
#include "commontypes.hpp"
#include "phasedScore.hpp"
#include "pst.hpp"
#include <libchess/position.hpp>
namespace evaluation {

int evaluate(const libchess::Position &pos, const int ply = 0,
             const Options opts = Options(0),
             const std::array<PhaseScore, 64 * 6> &ftable = final_table,
             const std::array<PhaseScore, 6> &pVal = pieceVal);
std::pair<uint8_t, uint8_t> count_checks(libchess::Position pos,
                                         const Options &opts);
const int mate_score = 25000;
constexpr int mate_threshhold = mate_score - 1000;
constexpr PhaseScore divMask = (((1 << 12) - 1) << 16) | (((1 << 12) - 1));
const int contempt = 0;
constexpr libchess::Bitboard winKOTH =
    (libchess::bitboards::FileD | libchess::bitboards::FileE) &
    (libchess::bitboards::Rank4 | libchess::bitboards::Rank5);

} // namespace evaluation
#endif