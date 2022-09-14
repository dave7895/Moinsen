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

int negamax(libchess::Position &pos, int depth, int ply,
            libchess::Move &returnMove, std::atomic_bool &stop,
            const std::chrono::time_point<std::chrono::system_clock> &stopTime,
            info::searchInfo &sInfo, const Options opts) {
  // std::cout << "fen " << pos.get_fen() << std::endl;
  sInfo.nodecount++;
  if (ply - 1 > sInfo.seldepth) {
    sInfo.seldepth = ply - 1;
  }
  if (depth <= 0 || pos.legal_moves().empty()|| isterminal(pos, opts)) {
    return evaluation::evaluate(pos, ply, opts);
  } else if (pos.fiftymoves() || pos.threefold()) {
    return evaluation::contempt;
  }
  int topEval = std::numeric_limits<int>::min();
  auto moves = pos.legal_moves();
  auto topMove = moves[0];
  for (const auto &move : moves) {
    if (stop) {
      return 0;
    } else if (std::chrono::system_clock::now() >
               stopTime + std::chrono::milliseconds(50)) {
      stop = true;
      return topEval;
    }
    pos.makemove(move);
    libchess::Move m;
    auto eval = -negamax(pos, depth - 1, ply + 1, m, stop, stopTime, sInfo, opts);
    pos.undomove();
    if (eval > topEval) {
      topEval = eval;
      topMove = move;
    }
  }
  returnMove = topMove;
  return topEval;
}

libchess::Move iterative_deepening(libchess::Position &pos,
                                   const std::chrono::milliseconds minTime,
                                   std::atomic_bool &stop, const Options opts) {
  int depth = 0;
  libchess::Move top_move = pos.legal_moves()[0];
  libchess::Move old_top_move;
  int score;
  const auto starttime = std::chrono::system_clock::now();
  const auto breakTime = starttime + (3 * minTime / 2);
  while ((std::chrono::system_clock::now() - starttime) < minTime && !stop) {
    depth++;
    info::searchInfo sInfo;
    score = negamax(pos, depth, 0, old_top_move, stop, breakTime, sInfo, opts);
    if (!stop) {
      top_move = old_top_move;
      long time = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::system_clock::now() - starttime)
                      .count();
      info::infopr(depth, score, old_top_move, time, sInfo);
    }
  }
  std::cout << "info string " << +opts.counts.first << " " << +opts.counts.second << " " << opts.move << " " << pos.history().size() << std::endl;
  std::cout << "bestmove " << top_move << std::endl;
  return top_move;
}
} // namespace search