#include "../src/search.hpp"
#include "doctest.h"
#include <array>
#include <chrono>
TEST_SUITE_BEGIN("Search");

TEST_CASE("Search - Movetime") {
  constexpr std::array<int, 3> movetimes = {20, 100, 200};

  std::atomic<bool> stop = false;
  libchess::Position pos("startpos");
  std::cout << "now inside movetime test" << std::endl;
  for (const auto movetime : movetimes) {
    const auto t0 = std::chrono::steady_clock::now();
    const auto bestmove = search::iterative_deepening(pos, std::chrono::milliseconds(movetime / 20), stop);
    const auto t1 = std::chrono::steady_clock::now();
    const auto dt =
        std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    REQUIRE(dt.count() <= movetime + 50);
    REQUIRE(bestmove != libchess::Move());
  }
  std::cout << "movetime tests over" << std::endl;
}

TEST_SUITE_END();