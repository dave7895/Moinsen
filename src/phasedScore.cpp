#include "phasedScore.hpp"
int getPhase(const libchess::Position &pos) {
  return pos.occupancy(libchess::Knight).count() *
             phaseValues[libchess::Knight] +
         pos.occupancy(libchess::Bishop).count() *
             phaseValues[libchess::Bishop] +
         pos.occupancy(libchess::Rook).count() * phaseValues[libchess::Rook] +
         pos.occupancy(libchess::Queen).count() * phaseValues[libchess::Queen];
}
