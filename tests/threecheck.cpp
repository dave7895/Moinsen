#include "../src/commontypes.hpp"
#include "../src/search.hpp"
#include "doctest.h"
#include <array>
#include <chrono>
#include <libchess/position.hpp>

TEST_SUITE_BEGIN("3check");
using pair_type = std::pair<std::string, std::string>;
const auto stopT = std::chrono::system_clock::now() + std::chrono::hours(1);
std::vector<std::vector<libchess::Move>> moveS;

TEST_CASE("Mate by 3 checks, one remaining") {
  const std::array<pair_type, 4> tests = {{
      {"2k4r/pppq2p1/2p1b3/7p/1B3Q2/4RN2/1P1PPPPP/2K2B1R b - - 3+1 2 20",
       "d7d2"},
      {"r1b3nr/ppqn1kp1/4p2p/1N6/P7/2p1P3/1P1B1PPP/R3KB1R w KQ - 1+2 0 10",
       "b5d6"},
      {"rk6/2b4R/1p6/p3p3/P1pP4/4P3/2P1K3/8 w - - 1+2 0 30", "h7h8"},
      {"2b4r/1R3pp1/n1pk3p/3p4/3P4/6P1/3nPP1P/4KBNR w K - 1+2 1 16", "b7d7"},
  }};
  info::searchInfo sinfo;
  int depth = 3;
  std::atomic<bool> stop = false;
  Options opts(threecheck);
  std::vector<libchess::Move> m(depth * depth);
  for (const auto &[fen, movestr] : tests) {
    auto newfen = extract_std_fen(fen, opts);
    libchess::Position pos(newfen);
    INFO(fen);
    INFO(pos.get_fen());
    INFO("given checks: w ", +opts.counts.first, " b ", +opts.counts.second);
    search::negamax(pos, depth, 0, m, stop, stopT, sinfo, moveS, opts);
    REQUIRE(static_cast<std::string>(m[0]) == movestr);
  }
}
