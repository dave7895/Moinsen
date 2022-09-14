#include "commontypes.hpp"
#include <iostream>

std::string extract_std_fen(std::string fen, Options &opts) {
  std::string collect;
  std::string placeholder;
  opts.move = 0;
  std::stringstream sstr{fen};
  while (sstr >> placeholder) {
    if (placeholder[1] == '+') {
      std::cout << "found chess.py 3check fen" << std::endl;
      std::cout << "fen: " << fen << std::endl;
      std::cout << "placeholder: " << placeholder << std::endl;
      opts.counts.first = static_cast<uint8_t>('3' - placeholder[0]);
      opts.counts.second = static_cast<uint8_t>('3' - placeholder[2]);
    } else if (placeholder[0] == '+') {
      opts.counts.first = static_cast<uint8_t>(placeholder[1] - '0');
      opts.counts.second = static_cast<uint8_t>(placeholder[3] - '0');
    } else {
      if (!collect.empty()) {
        collect += " ";
      }
      collect += placeholder;
    }
  }
  return collect;
}
