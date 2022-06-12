#include "../src/uci.hpp"
#include "doctest.h"
#include <array>
#include <sstream>

TEST_CASE("UCI Position"){
   const std::array<std::string, 1> tests= {{"8/5PP1/1p4P1/N1b5/p5p1/1K6/5k2/8 b - - 0 10"}};
   libchess::Position pos;
   for (const auto &str: tests){
      std::stringstream sstr{str};
      uci::position(sstr, pos);
      REQUIRE(pos.get_fen() == str);
   }
}