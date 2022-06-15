#include "info.hpp"
namespace info {
void infopr(int depth, int score, libchess::Move top_move, long time,
            const searchInfo &sInfo) {
  long nps = (sInfo.nodecount * 1000) / std::max(time, 1L);
  std::cout << "info depth " << depth << " seldepth " << sInfo.seldepth
            << " score cp " << score << " pv "
            << static_cast<std::string>(top_move) << " time " << time << " nodes "
            << sInfo.nodecount << " nps " << nps << std::endl;
}
} // namespace info
