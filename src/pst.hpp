#ifndef PST_HPP
#define PST_HPP
#include <array>
#include <libchess/position.hpp>
namespace evaluation {
template <libchess::Side us> int evaluatePstSide(const libchess::Position &pos);
int evaluatePST(libchess::Position &pos);
} // namespace evaluation

#endif
