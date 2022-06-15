#include "info.hpp"
namespace info {
void infopr(int depth, int score, libchess::Move top_move, long time,
            const searchInfo &sInfo) {
  long nps = (sInfo.nodecount * 1000) / std::max(time, 1L);
  std::cout << "info depth " << depth << " seldepth " << sInfo.seldepth
            << " score ";
  bool is_mate = score % 10000 == 0 && (score != 0);
  if (is_mate) {
    int mate_score = (score / 10000);
    mate_score = std::max(1, depth -1 - mate_score);
    std::cout << "mate " << mate_score;
  } else {
    std::cout << "cp " << score;
  }
  std::cout << " time " << time << " nodes " << sInfo.nodecount << " nps "
            << nps << " pv " << static_cast<std::string>(top_move) << std::endl;
}
} // namespace info
