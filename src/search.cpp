#include "search.hpp"
namespace search {
bool isterminal(libchess::Position pos, const Options opts) {
  if (pos.is_terminal()) {
    return true;
  } else if (opts.var == kingofthehill) {
    return !(pos.occupancy(libchess::King) & evaluation::winKOTH).empty();
  } else if (opts.var == threecheck) {
    const auto check_count = evaluation::count_checks(pos, opts);
    return (check_count.first >= 3 || check_count.second >= 3);
  }
  return false;
}

int negamax(libchess::Position &pos, int depth, const unsigned int ply,
            std::vector<libchess::Move> &returnMove, std::atomic_bool &stop,
            const std::chrono::time_point<std::chrono::system_clock> &stopTime,
            info::searchInfo &sInfo,
            std::vector<std::vector<libchess::Move>> &moveStack,
            const Options opts) {
  // std::cout << "fen " << pos.get_fen() << std::endl;
  sInfo.nodecount++;
  if (ply > sInfo.seldepth) {
    sInfo.seldepth = ply;
  }
  while (moveStack.size() <= ply) {
    moveStack.emplace_back();
    moveStack.back().reserve(150);
  }
  if (depth <= 0 || isterminal(pos, opts)) {
    return evaluation::evaluate(pos, ply, opts);
  } else if (pos.fiftymoves() || pos.threefold()) {
    return evaluation::contempt;
  }
  int topEval = std::numeric_limits<int>::min();
  auto moves = moveStack[ply];
  moves.clear();
  pos.legal_moves(moves);
  const int N = depth + ply;
  const int PVzeroOffset = ply * (2 * N + 1 - ply) / 2;
  for (const auto &move : moves) {
    if (stop) {
      return 0;
    } else if (std::chrono::system_clock::now() >
               stopTime + std::chrono::milliseconds(50)) {
      stop = true;
      return topEval;
    }
    pos.makemove(move);
    auto eval = -negamax(pos, depth - 1, ply + 1, returnMove, stop, stopTime,
                         sInfo, moveStack, opts);
    pos.undomove();
    if (eval > topEval) {
      topEval = eval;
      for (int i = PVzeroOffset; i < PVzeroOffset + depth - 1; i++) {
        returnMove[i + 1] = returnMove[i + (N - ply)];
      }
      returnMove[PVzeroOffset] = move;
    }
  }
  return topEval;
}

// TODO: add vec of vecs to not allocate movelist in tight loop

libchess::Move iterative_deepening(libchess::Position &pos,
                                   const std::chrono::milliseconds minTime,
                                   std::atomic_bool &stop, const Options opts) {
  int depth = 0;
  std::vector<std::vector<libchess::Move>> moveStack(4, {pos.legal_moves()});
  std::vector<libchess::Move> top_move = {moveStack[0][0]};
  libchess::Move old_top_move;
  int score;
  const auto starttime = std::chrono::system_clock::now();
  const auto breakTime = starttime + (3 * minTime / 2);
  while ((std::chrono::system_clock::now() - starttime) < minTime && !stop) {
    depth++;
    moveStack.emplace_back();
    // moveStack.back().reserve(200);
    std::vector<libchess::Move> newPV((depth * depth + depth) / 2);
    info::searchInfo sInfo;
    score =
        negamax(pos, depth, 0, newPV, stop, breakTime, sInfo, moveStack, opts);
    if (!stop) {
      top_move = newPV;
      long time = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::system_clock::now() - starttime)
                      .count();
      info::infopr(depth, score, top_move, time, sInfo);
    }
  }
  std::cout << "info string " << +opts.counts.first << " "
            << +opts.counts.second << " " << opts.move << " "
            << pos.history().size() << std::endl;
  std::cout << "bestmove " << top_move[0] << std::endl;
  return top_move[0];
}
} // namespace search
