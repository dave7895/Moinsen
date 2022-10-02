#ifndef PST_HPP
#define PST_HPP
#include "correctPval.hpp"
#include "phasedScore.hpp"
#include <algorithm>
#include <array>
#include <libchess/position.hpp>
namespace evaluation {
extern const std::array<PhaseScore, 64 * 6> final_table;
template <libchess::Side us>
PhaseScore
evaluatePstSide(const libchess::Position &pos,
                const std::array<PhaseScore, 64 * 6> &ftable = final_table);
// int evaluatePST(libchess::Position &pos);
} // namespace evaluation

#endif
