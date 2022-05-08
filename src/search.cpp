#include "search.hpp"
namespace search {
int negamax(libchess::Position &pos, int depth, int ply,
            libchess::Move &returnMove, std::atomic_bool &stop) {
  // std::cout << "fen " << pos.get_fen() << std::endl;
  if (depth <= 0 || pos.legal_moves().empty()) {
    return evaluation::evaluate(pos);
  }
  int topEval = std::numeric_limits<int>::min();
  auto moves = pos.legal_moves();
  auto topMove = moves[0];
  for (const auto &move : moves) {
    if (stop)
      return 0;
    pos.makemove(move);
    libchess::Move m;
    auto eval = -negamax(pos, depth - 1, ply + 1, m, stop);
    pos.undomove();
    if (eval > topEval) {
      topEval = eval;
      topMove = move;
    }
  }
  returnMove = topMove;
  return topEval;
}

libchess::Move iterative_deepening(
    libchess::Position &pos,
    const std::chrono::time_point<std::chrono::system_clock> stopTime,
    std::atomic_bool &stop) {
  int depth = 1;
  libchess::Move top_move;
  libchess::Move old_top_move;
  int score;
  while (std::chrono::system_clock::now() < stopTime && !stop) {
    depth++;
    score = negamax(pos, depth, 1, old_top_move, stop);
    if (!stop) {
      top_move = old_top_move;
    }
    std::cout << "info depth " << depth << " score cp " << score << " pv "
              << old_top_move << std::endl;
  }
  std::cout << "bestmove " << top_move << std::endl;
  return top_move;
}
} // namespace search