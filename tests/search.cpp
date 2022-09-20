#include "../src/search.hpp"
#include "doctest.h"
#include <array>
#include <chrono>
#include <libchess/position.hpp>

TEST_SUITE_BEGIN("Search");
using pair_type = std::pair<std::string, std::string>;
const auto stopT = std::chrono::system_clock::now() + std::chrono::hours(1);
info::searchInfo sInfo;
TEST_CASE("Mate in one") {
  const std::array<pair_type, 4> tests = {{
      {"3k4/8/3K4/8/5R2/8/8/8 w - - 0 1", "f4f8"},
      {"8/8/8/5r2/8/3k4/8/3K4 b - - 0 1", "f5f1"},
      {"1k1r2R1/8/1K6/8/8/8/8/8 w - - 0 1", "g8d8"},
      {"8/8/8/8/8/1k6/8/1K1R2r1 b - - 0 1", "g1d1"},
  }};
  int depth = 3;
  std::atomic<bool> stop = false;
  std::vector<libchess::Move> m(depth * depth);
  for (const auto &[fen, movestr] : tests) {
    libchess::Position pos(fen);
    search::negamax(pos, depth, 0, m, stop, stopT, sInfo);
    REQUIRE(static_cast<std::string>(m[0]) == movestr);
  }
}

TEST_CASE("Search - Checkmate with castling") {
  const std::array<pair_type, 4> tests = {{
      {"8/8/8/8/8/8/7R/1k2K2R w K - 0 1", "e1g1"},
      {"1K2k2r/7r/8/8/8/8/8/8 b k - 0 1", "e8g8"},
      {"8/8/8/8/8/8/R7/R3K2k w Q - 0 1", "e1c1"},
      {"r3k2K/r7/8/8/8/8/8/8 b q - 0 1", "e8c8"},
  }};

  int depth = 3;
  std::atomic<bool> stop = false;
  std::vector<libchess::Move> m(depth * depth);
  for (const auto &[fen, movestr] : tests) {
    libchess::Position pos(fen);
    const int eval = search::negamax(pos, depth, 0, m, stop, stopT, sInfo);
    INFO(fen);
    INFO("eval ", eval);
    REQUIRE(static_cast<std::string>(m[0]) == movestr);
  }
}

TEST_SUITE_END();