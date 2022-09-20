#ifndef PST_HPP
#define PST_HPP
#include "phasedScore.hpp"
#include <algorithm>
#include <array>
#include <libchess/position.hpp>
namespace evaluation {
template <libchess::Side us> PhaseScore evaluatePstSide(const libchess::Position &pos);
int evaluatePST(libchess::Position &pos);
} // namespace evaluation

#endif
