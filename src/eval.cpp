#include "eval.hpp"
namespace evaluation {

template <libchess::Side us>
int evaluate_us(const libchess::Position &pos) {
  int eval = 0;
  for (const auto &p : libchess::pieces) {
    const int multiplier = pieceToVal[p];
    eval += multiplier * pos.pieces(us, p).count();
  }
  return eval;
}

int evaluate(const libchess::Position &pos, const int depth) {
  if (pos.count_moves() == 0) {
    return pos.in_check() ? -mate_score * (depth + 1) : 0;
  } else if (pos.threefold() || pos.fiftymoves()) {
    return 0;
  }
  int eval = 0;
  eval += evaluate_us<libchess::Side::White>(pos);
  eval -= evaluate_us<libchess::Side::Black>(pos);
  return (1 - 2 * pos.turn()) * eval;
}
} // namespace evaluation