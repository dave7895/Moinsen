#include "eval.hpp"
namespace evaluation {
int evaluate(const libchess::Position &pos) {
  if (pos.count_moves() == 0) {
    return pos.in_check() ? -mate_score : 0;
  } else if (pos.threefold() || pos.fiftymoves())
    return 0;
  int eval = 0;
  for (const auto &p : libchess::pieces) {
    const int multiplier = pieceToVal[p];
    eval += multiplier * pos.pieces(pos.turn(), p).count();
    eval -= multiplier * pos.pieces(!pos.turn(), p).count();
  }
  return eval;
}
} // namespace evaluation