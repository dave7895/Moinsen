#include "pst.hpp"

namespace evaluation {

constexpr std::array<PhaseScore, 64 * 6> init_table() {
  std::array<PhaseScore, 64 * 6> compound_table;
  std::transform(mg_table.begin(), mg_table.end(), eg_table.begin(),
                 compound_table.begin(), [](const int a, const int b) {
                   return S(static_cast<int16_t>(a), static_cast<int16_t>(b));
                 });
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 64; j++) {
      compound_table[i * 64 + j] += pieceVal[i];
    }
  }
  return compound_table;
}

constexpr std::array<PhaseScore, 64 * 6> final_table = init_table();

template <libchess::Piece piece>
PhaseScore pstValue(const libchess::Square &sq,
                    const std::array<PhaseScore, 64 * 6> &ftable) {
  return ftable[64 * piece + static_cast<int>(sq)];
}

template <libchess::Side us, libchess::Piece ptype>
PhaseScore evaluatePstPiece(const libchess::Position &pos,
                            const std::array<PhaseScore, 64 * 6> &ftable) {
  PhaseScore eval = 0;
  for (const auto &sq : pos.pieces(us, ptype)) {
    if constexpr (us == libchess::Side::White) {
      eval += pstValue<ptype>(sq.flip(), ftable);
    } else {
      eval += pstValue<ptype>(sq, ftable);
    }
  }
  return eval;
}

template <libchess::Side us>
PhaseScore evaluatePstSide(const libchess::Position &pos,
                           const std::array<PhaseScore, 64 * 6> &ftable) {
  PhaseScore eval = 0;
  eval += evaluatePstPiece<us, libchess::Pawn>(pos, ftable);
  eval += evaluatePstPiece<us, libchess::Knight>(pos, ftable);
  eval += evaluatePstPiece<us, libchess::Bishop>(pos, ftable);
  eval += evaluatePstPiece<us, libchess::Rook>(pos, ftable);
  eval += evaluatePstPiece<us, libchess::Queen>(pos, ftable);
  eval += evaluatePstPiece<us, libchess::King>(pos, ftable);
  return eval;
}

PhaseScore
evaluatePST(const libchess::Position &pos,
            const std::array<PhaseScore, 64 * 6> &ftable = final_table) {
  PhaseScore eval = 0;
  eval += evaluatePstSide<libchess::Side::White>(pos, ftable);
  eval -= evaluatePstSide<libchess::Side::Black>(pos, ftable);
  return pos.turn() == libchess::Side::White ? eval : -eval;
}
} // namespace evaluation
