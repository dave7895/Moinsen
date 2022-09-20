#ifndef PHASESCORE_HPP
#define PHASESCORE_HPP
#include <cstdint>
#include <libchess/position.hpp>

typedef int32_t PhaseScore;
typedef int16_t Score;

constexpr Score mgScore(PhaseScore score) {
  return static_cast<int16_t>(static_cast<uint16_t>(static_cast<unsigned>(score)));
}

constexpr Score egScore(PhaseScore score) {
  return static_cast<int16_t>(static_cast<uint16_t>(static_cast<unsigned>(score + 0x8000) >> 16));
}

constexpr PhaseScore S(const int16_t mg, const int16_t eg) {
  return static_cast<PhaseScore>(static_cast<uint32_t>(eg) << 16) + mg;
}

constexpr std::array<int, 5> phaseValues{0, 1, 1, 2, 4};

int getPhase(const libchess::Position &pos);
#endif
