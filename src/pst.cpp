#include "pst.hpp"

namespace evaluation {

// tables copied from
// https://www.chessprogramming.org/Simplified_Evaluation_Function

// clang-format off
std::array<int, 64*6> table = {{
//pawns
0,  0,  0,  0,  0,  0,  0,  0,
50, 50, 50, 50, 50, 50, 50, 50,
10, 10, 20, 30, 30, 20, 10, 10,
5,  5, 10, 25, 25, 10,  5,  5,
0,  0,  0, 20, 20,  0,  0,  0,
5, -5,-10,  0,  0,-10, -5,  5,
5, 10, 10,-20,-20, 10, 10,  5,
0,  0,  0,  0,  0,  0,  0,  0,
 //knights
 -50,-40,-30,-30,-30,-30,-40,-50,
-40,-20,  0,  0,  0,  0,-20,-40,
-30,  0, 10, 15, 15, 10,  0,-30,
-30,  5, 15, 20, 20, 15,  5,-30,
-30,  0, 15, 20, 20, 15,  0,-30,
-30,  5, 10, 15, 15, 10,  5,-30,
-40,-20,  0,  5,  5,  0,-20,-40,
-50,-40,-30,-30,-30,-30,-40,-50,
// bishop
-20,-10,-10,-10,-10,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5, 10, 10,  5,  0,-10,
-10,  5,  5, 10, 10,  5,  5,-10,
-10,  0, 10, 10, 10, 10,  0,-10,
-10, 10, 10, 10, 10, 10, 10,-10,
-10,  5,  0,  0,  0,  0,  5,-10,
-20,-10,-10,-10,-10,-10,-10,-20,
// rook
0,  0,  0,  0,  0,  0,  0,  0,
5, 10, 10, 10, 10, 10, 10,  5,
-5,  0,  0,  0,  0,  0,  0, -5,
-5,  0,  0,  0,  0,  0,  0, -5,
-5,  0,  0,  0,  0,  0,  0, -5,
-5,  0,  0,  0,  0,  0,  0, -5,
-5,  0,  0,  0,  0,  0,  0, -5,
0,  0,  0,  5,  5,  0,  0,  0,
  //queen
-20,-10,-10, -5, -5,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5,  5,  5,  5,  0,-10,
 -5,  0,  5,  5,  5,  5,  0, -5,
  0,  0,  5,  5,  5,  5,  0, -5,
-10,  5,  5,  5,  5,  5,  0,-10,
-10,  0,  5,  0,  0,  0,  0,-10,
-20,-10,-10, -5, -5,-10,-10,-20,
// king
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-20,-30,-30,-40,-40,-30,-30,-20,
-10,-20,-20,-20,-20,-20,-20,-10,
 20, 20,  0,  0,  0,  0, 20, 20,
 20, 30, 10,  0,  0, 10, 30, 20,
}};
// clang-format on
template <libchess::Piece piece> int pstValue(libchess::Square const &sq) {
  return table[64 * piece + static_cast<int>(sq)];
}

template <libchess::Side us, libchess::Piece ptype>
int evaluatePstPiece(const libchess::Position &pos) {
  int eval = 0;
  for (const auto &sq : pos.pieces(us, ptype)) {
    if constexpr (us == libchess::Side::White) {
      eval += pstValue<ptype>(sq.flip());
    } else {
      eval += pstValue<ptype>(sq);
    }
  }
  return eval;
}

template <libchess::Side us>
int evaluatePstSide(const libchess::Position &pos) {
  int eval = 0;
  eval += evaluatePstPiece<us, libchess::Pawn>(pos);
  eval += evaluatePstPiece<us, libchess::Knight>(pos);
  eval += evaluatePstPiece<us, libchess::Bishop>(pos);
  eval += evaluatePstPiece<us, libchess::Rook>(pos);
  eval += evaluatePstPiece<us, libchess::Queen>(pos);
  eval += evaluatePstPiece<us, libchess::King>(pos);
  return eval;
}

int evaluatePST(const libchess::Position &pos) {
  int eval = 0;
  eval += evaluatePstSide<libchess::Side::White>(pos);
  eval -= evaluatePstSide<libchess::Side::Black>(pos);
  return pos.turn() == libchess::Side::White ? eval : -eval;
}
} // namespace evaluation
