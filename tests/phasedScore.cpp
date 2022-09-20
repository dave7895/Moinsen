#include "../src/phasedScore.hpp"
#include "doctest.h"
#include <array>
#include <chrono>

TEST_SUITE_BEGIN("PhasedScore");
using pair_type = std::pair<int16_t, int16_t>;
TEST_CASE("Extraction") {
  const std::array<pair_type, 4> tests = {{
      {100, 100},
      {100, -150},
      {-150, 100},
      {-200, -200},
  }};
  for (const auto &[mg, eg] : tests) {
    auto pScore = S(mg, eg);
    INFO(mg, " ", eg);
    REQUIRE(mgScore(pScore) == mg);
    REQUIRE(egScore(pScore) == eg);
  }
}

TEST_CASE("Additive Inverse") {
  const std::array<pair_type, 4> tests = {{
      {100, 100},
      {100, -150},
      {-150, 100},
      {-200, -200},
  }};
  for (const auto &[mg, eg] : tests) {
    auto pScore = S(mg, eg);
    INFO(mg, " ", eg);
    REQUIRE(mgScore(-pScore) == -mg);
    REQUIRE(egScore(-pScore) == -eg);
  }
}

TEST_CASE("Addition") {
  const std::array<pair_type, 4> tests = {{
      {100, 100},
      {100, -150},
      {-150, 100},
      {-200, -200},
  }};
  for (const auto &[mg1, eg1] : tests) {
    auto pScore1 = S(mg1, eg1);
    for (const auto &[mg2, eg2] : tests) {
      auto pScore2 = S(mg2, eg2);
      const auto combined = pScore1 + pScore2;
      INFO(mg1, " ", eg1, " ", mg2, " ", eg2);
      REQUIRE(mgScore(combined) == mg1 + mg2);
      REQUIRE(egScore(combined) == eg1 + eg2);
    }
  }
}

TEST_CASE("Multiplication") {
  const std::array<pair_type, 4> tests = {{
      {100, 100},
      {100, -150},
      {-150, 100},
      {-200, -200},
  }};
  for (const auto &[mg, eg] : tests) {
    auto pScore = S(mg, eg);
    INFO(mg, " ", eg);
    REQUIRE(mgScore(5 * pScore) == 5 * mg);
    REQUIRE(egScore(5 * pScore) == 5 * eg);
  }
}
