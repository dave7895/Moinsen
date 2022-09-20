#ifndef EVALUATION_HPP
#define EVALUATION_HPP
#include "commontypes.hpp"
#include "phasedScore.hpp"
#include "pst.hpp"
#include <libchess/position.hpp>
namespace evaluation {
int evaluate(const libchess::Position &pos, const int ply = 1,
             Options opts = Options(1));
std::pair<uint8_t, uint8_t> count_checks(libchess::Position pos,
                                         const Options &opts);
const int mate_score = 25000;
constexpr int mate_threshhold = mate_score - 1000;
constexpr PhaseScore divMask = (((1 << 12) - 1) << 16) | (((1 << 12) - 1));
const int contempt = 0;
const std::vector<PhaseScore> pieceToVal = {
    S(100, 105), S(310, 305), S(330, 320), S(490, 500), S(900, 905), S(0, 0)};
constexpr libchess::Bitboard winKOTH =
    (libchess::bitboards::FileD | libchess::bitboards::FileE) &
    (libchess::bitboards::Rank4 | libchess::bitboards::Rank5);

} // namespace evaluation
#endif