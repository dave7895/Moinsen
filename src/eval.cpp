#include "eval.hpp"
#include "iostream"
namespace evaluation {

int evaluate_checks(const libchess::Position &pos, const int ply,
                    const Options &opts);

template <libchess::Side us>
PhaseScore evaluate_us(const libchess::Position &pos,
                       const std::array<PhaseScore, 64 * 6> &ftable,
                       const std::array<PhaseScore, 6> &pVal) {
  PhaseScore eval = 0;
  // const auto our = pos.occupancy(us);
  const auto opp = pos.occupancy(!us);
  const auto attacked = pos.squares_attacked(us);
  const int attackDiscount = 8;

  const auto pawns = pos.occupancy(libchess::Pawn);
  // eval += pVal[0] * (pawns & our).count();
  eval +=
      (pVal[0] / attackDiscount * (pawns & opp & attacked).count()) & divMask;

  const auto knights = pos.occupancy(libchess::Knight);
  // eval += pVal[1] * (knights & our).count();
  eval +=
      (pVal[1] / attackDiscount * (knights & opp & attacked).count()) & divMask;

  const auto bishops = pos.occupancy(libchess::Bishop);
  // eval += pVal[2] * (bishops & our).count();
  eval +=
      (pVal[2] / attackDiscount * (bishops & opp & attacked).count()) & divMask;

  const auto rooks = pos.occupancy(libchess::Rook);
  // eval += pVal[3] * (rooks & our).count();
  eval +=
      (pVal[3] / attackDiscount * (rooks & opp & attacked).count()) & divMask;

  const auto queens = pos.occupancy(libchess::Queen);
  // eval += pVal[4] * (queens & our).count();
  eval +=
      (pVal[4] / attackDiscount * (queens & opp & attacked).count()) & divMask;

  eval += evaluatePstSide<us>(pos, ftable);
  // bishop pair
  eval += 15 * ((bishops & libchess::bitboards::DarkSquares) &&
                (bishops & libchess::bitboards::LightSquares));
  // value castling, low enough to not discourage it by valuing it lower than
  // castled PST
  eval += 2 * pos.can_castle(us, libchess::MoveType::ksc);
  eval += 2 * pos.can_castle(us, libchess::MoveType::qsc);
  return eval;
}

int evaluate(const libchess::Position &pos, const int ply, const Options opts,
             const std::array<PhaseScore, 64 * 6> &ftable,
             const std::array<PhaseScore, 6> &pVal) {
  if (pos.count_moves() == 0) {
    return pos.in_check() ? -mate_score + ply : contempt;
  } else if (pos.threefold() || pos.fiftymoves()) {
    return contempt;
  }

  PhaseScore phasedEval = 0;

  if (opts.var == threecheck) {
    const auto ceval = evaluate_checks(pos, ply, opts);
    if (ceval > mate_threshhold || ceval < -mate_threshhold) {
      return pos.turn() == libchess::Side::White ? ceval : -ceval;
    } else {
      phasedEval += S(static_cast<int16_t>(ceval), static_cast<int16_t>(ceval));
    }
  } else if (opts.var == kingofthehill) {
    const auto kingInMiddle =
        pos.occupancy(libchess::King) & evaluation::winKOTH;
    if (!kingInMiddle.empty()) {
      const int eval =
          (pos.occupancy(libchess::Side::White) & kingInMiddle).empty()
              ? -mate_score + ply
              : mate_score - ply;
      return pos.turn() == libchess::Side::White ? eval : -eval;
    }
  }

  phasedEval += evaluate_us<libchess::Side::White>(pos, ftable, pVal);
  phasedEval -= evaluate_us<libchess::Side::Black>(pos, ftable, pVal);
  constexpr int maxPhase = 24;
  const int gamePhase = std::min(getPhase(pos), maxPhase);
  const int midgame = mgScore(phasedEval);
  const int endgame = egScore(phasedEval);
  const int tapered =
      (midgame * gamePhase + (24 - gamePhase) * endgame) / maxPhase;
  const int perspectiveEval =
      pos.turn() == libchess::Side::White ? tapered : -tapered;
  return perspectiveEval + pos.in_check();
}

std::pair<uint8_t, uint8_t> count_checks(libchess::Position pos,
                                         const Options &opts) {
  std::pair<uint8_t, uint8_t> check_count = opts.counts;
  while (pos.history().size() > static_cast<size_t>(opts.move)) {
    if (pos.in_check()) {
      if (pos.turn()) {
        check_count.first++;
      } else {
        check_count.second++;
      }
    }
    pos.undomove();
  }
  return check_count;
}

int evaluate_checks(const libchess::Position &pos, const int ply,
                    const Options &opts) {
  const auto checks = count_checks(pos, opts);
  int eval = 0;
  eval += checks.first * checks.first * 300;
  eval -= checks.second * checks.second * 300;
  if (checks.first > 2) {
    return mate_score - ply;
  } else if (checks.second > 2) {
    return -mate_score + ply;
  }
  return eval;
}
} // namespace evaluation