#include "eval.hpp"
namespace evaluation {

template <libchess::Side us> int evaluate_us(const libchess::Position &pos) {
  int eval = 0;
  eval += pieceToVal[0] * pos.pieces(us, libchess::Pawn).count();
  eval += pieceToVal[1] * pos.pieces(us, libchess::Knight).count();
  eval += pieceToVal[2] * pos.pieces(us, libchess::Bishop).count();
  eval += pieceToVal[3] * pos.pieces(us, libchess::Rook).count();
  eval += pieceToVal[4] * pos.pieces(us, libchess::Queen).count();
  eval += evaluatePstSide<us>(pos);
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
  return pos.turn() == libchess::Side::White ? eval : -eval;
}
} // namespace evaluation