#include "uci.hpp"
#include <iostream>

int main() {
  std::string interfacemode;
  std::getline(std::cin, interfacemode);
  libchess::Position pos("startpos");
  libchess::Move m;
  std::atomic_bool stop = false;
  /*search::iterative_deepening(
      pos, std::chrono::system_clock::now() + std::chrono::milliseconds(1000),
      stop);*/
  if (interfacemode == "uci") {
    uci::main_loop();
  } else {
    return 1;
  }
  return 0;
}