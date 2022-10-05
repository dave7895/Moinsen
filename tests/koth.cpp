// tests for the kingofthehill variant

#include "../src/commontypes.hpp"
#include "../src/search.hpp"
#include "doctest.h"
#include <array>
#include <chrono>
#include <libchess/position.hpp>

TEST_SUITE_BEGIN("koth");
using pair_type = std::pair<std::string, std::string>;
const auto stopT = std::chrono::system_clock::now() + std::chrono::hours(1);
std::vector<std::vector<libchess::Move>> moveSt;

TEST_CASE("Mate by going into middle") {
  const std::array<pair_type, 4> tests = {{
      {
          "rnb1k1nr/1ppp2p1/p3p2p/6q1/1b2P3/4K3/PPP2PPP/RNBQ1BNR w kq - 2 7",
          "e3d4",
      },
      {
          "8/b4p1p/p1pk1p2/2Pr4/2K5/4r3/P6P/2R5 b - - 0 31",
          "d6e5",
      },
      {
          "rnb2r2/pp1p2Np/1bpk2p1/4pP2/QPB5/P1N2KP1/3P1P1P/R1B4R w - - 1 18",
          "f3e4",
      },
      {
          "3r4/p6p/2K2kp1/8/8/6P1/7P/8 b - - 4 61",
          "f6e5",
      },
  }};
  info::searchInfo sinfo;
  int depth = 3;
  std::atomic<bool> stop = false;
  Options opts(kingofthehill);
  std::vector<libchess::Move> m(depth * depth);
  for (const auto &[fen, movestr] : tests) {
    libchess::Position pos(fen);
    INFO(fen);
    INFO(pos.get_fen());
    search::negamax(pos, depth, 0, m, stop, stopT, sinfo, moveSt, opts);
    REQUIRE(static_cast<std::string>(m[0]) == movestr);
  }
}
