#include "../phasedScore.hpp"
#include <array>
#include <cstdint>

constexpr std::array<int16_t, 64 * 6> mg_table = {{}};
constexpr std::array<int16_t, 64 * 6> eg_table = {{}};
constexpr std::array<PhaseScore, 6> pieceVal = {{
    S(100, 100),
    S(300, 300),
    S(300, 300),
    S(500, 500),
    S(900, 900),
    S(0, 0),
}};
constexpr std::array<PhaseScore, 6> mobilityBonus = {{

}};
constexpr std::array<PhaseScore, 6> attackBonus = {{}};
