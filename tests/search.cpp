#include "../src/search.hpp"
#include "doctest.h"
#include <array>
#include <libchess/position.hpp>

TEST_SUITE_BEGIN("Search");
using pair_type = std::pair<std::string, std::string>;

TEST_CASE("Mate in one") {
  const std::array<pair_type, 4> tests = {{
      {"3k4/8/3K4/8/5R2/8/8/8 w - - 0 1", "f4f8"},
      {"8/8/8/5r2/8/3k4/8/3K4 b - - 0 1", "f5f1"},
      {"1k1r2R1/8/1K6/8/8/8/8/8 w - - 0 1", "g8d8"},
      {"8/8/8/8/8/1k6/8/1K1R2r1 b - - 0 1", "g1d1"},
  }};
  int depth = 3;
  std::atomic<bool> stop = false;
  for (const auto &[fen, movestr] : tests) {
    libchess::Position pos(fen);
    libchess::Move m;
    search::negamax(pos, depth, 1, m, stop);
    REQUIRE(static_cast<std::string>(m) == movestr);
  }
}

TEST_CASE("Search - Checkmate with castling") {
    const std::array<pair_type, 4> tests = {{
        {"8/8/8/8/8/8/7R/1k2K2R w K - 0 1", "e1h1"},
        {"1K2k2r/7r/8/8/8/8/8/8 b k - 0 1", "e8h8"},
        {"8/8/8/8/8/8/R7/R3K2k w Q - 0 1", "e1a1"},
        {"r3k2K/r7/8/8/8/8/8/8 b q - 0 1", "e8a8"},
    }};

    int depth = 3;
  std::atomic<bool> stop = false;
  for (const auto &[fen, movestr] : tests) {
    libchess::Position pos(fen);
    libchess::Move m;
    search::negamax(pos, depth, 1, m, stop);
    INFO(fen);
    CHECK(static_cast<std::string>(m) == movestr);
  }
}

TEST_SUITE_END();