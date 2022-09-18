#include "info.hpp"
namespace info {
void infopr(const int depth, const int score,
            const std::vector<libchess::Move> &top_move, const long time,
            const searchInfo &sInfo) {
  long nps = (sInfo.nodecount * 1000) / std::max(time, 1L);
  std::cout << "info depth " << depth << " seldepth " << sInfo.seldepth
            << " score ";
  bool is_mate = (score > evaluation::mate_threshhold) ||
                 (score < -evaluation::mate_threshhold);
  if (is_mate) {
    const int mate_in_plies = evaluation::mate_score - std::abs(score);
    const int mate_in_moves = (mate_in_plies + 1) / 2;
    std::cout << "mate " << (score < 0 ? "-" : "") << mate_in_moves;
  } else {
    std::cout << "cp " << score;
  }
  std::cout << " time " << time << " nodes " << sInfo.nodecount << " nps "
            << nps << " pv ";
  int counter = 0;
  for (const auto &move : top_move) {
    if (!move) {
      break;
    }
    std::cout << static_cast<std::string>(move) << " ";
    counter++;
    if (counter >= depth) {
      break;
    }
  }

  std::cout << std::endl;
}
} // namespace info
