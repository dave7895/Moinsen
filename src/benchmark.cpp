#include "benchmark.hpp"

namespace bench
{
   void run(std::string fen){
      libchess::Position pos{fen};
      std::chrono::milliseconds time{15000};
      libchess::Move bestMove;
      std::atomic_bool stop = false;
      search::iterative_deepening(pos, time, stop);
   }

} // namespace bench
