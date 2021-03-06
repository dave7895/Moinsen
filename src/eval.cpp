#include "eval.hpp"
namespace evaluation {

template <libchess::Side us> int evaluate_us(const libchess::Position &pos) {
  int eval = 0;
  const auto our = pos.occupancy(us);
  const auto opp = pos.occupancy(!us);
  const auto attacked = pos.squares_attacked(us);
  const int attackDiscount = 8;

  const auto pawns = pos.occupancy(libchess::Pawn);
  eval += pieceToVal[0] * (pawns & our).count();
  eval += pieceToVal[0] / attackDiscount * (pawns & opp & attacked).count();

  const auto knights = pos.occupancy(libchess::Knight);
  eval += pieceToVal[1] * (knights & our).count();
  eval += pieceToVal[1] / attackDiscount * (knights & opp & attacked).count();

  const auto bishops = pos.occupancy(libchess::Bishop);
  eval += pieceToVal[2] * (bishops & our).count();
  eval += pieceToVal[2] / attackDiscount * (bishops & opp & attacked).count();

  const auto rooks = pos.occupancy(libchess::Rook);
  eval += pieceToVal[3] * (rooks & our).count();
  eval += pieceToVal[3] / attackDiscount * (rooks & opp & attacked).count();

  const auto queens = pos.occupancy(libchess::Queen);
  eval += pieceToVal[4] * (queens & our).count();
  eval += pieceToVal[4] / attackDiscount * (queens & opp & attacked).count();

  eval += evaluatePstSide<us>(pos);
  // bishop pair
  eval += 15 * (bishops.count() > 1);
  // value castling, low enough to not discourage it by valuing it lower than
  // castled PST
  eval += 2 * pos.can_castle(us, libchess::MoveType::ksc);
  eval += 2 * pos.can_castle(us, libchess::MoveType::qsc);
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
  eval += rand() % 5 - 3;
  int perspectiveEval = pos.turn() == libchess::Side::White ? eval : -eval;
  return perspectiveEval + pos.in_check();
}
} // namespace evaluation