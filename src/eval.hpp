#ifndef EVALUATION_HPP
#define EVALUATION_HPP
#include <libchess/position.hpp>
namespace evaluation {
int evaluate(const libchess::Position &pos, const int depth = 1);
const int mate_score = 10000;
const std::vector<int> pieceToVal = {100, 310, 330, 500, 900, 0};
} // namespace evaluation
#endif