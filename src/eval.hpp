#ifndef EVALUATION_HPP
#define EVALUATION_HPP
#include "../libchess/src/libchess/position.hpp"
#include <libchess/position.hpp>
#include <map>
namespace evaluation {
int evaluate(const libchess::Position &pos, const int depth = 1);
const int mate_score = 10000;
const std::map<std::string, int> init{
    {"this", 100},
    {"can", 100},
    {"be", 100},
    {"const", 100},
};
const std::vector<int> pieceToVal = {100, 300, 300, 500, 900, 0};
} // namespace evaluation
#endif