#include "benchmark.hpp"
#include "uci.hpp"
#include <iostream>

int main(int argc, char **argv) {
  if (argc > 1) {
    std::string firstArg{argv[1]};
    std::cout << firstArg << std::endl;
    if (firstArg == "bench") {
      bench::run();
      return 0;
    }
  }
  std::string interfacemode;
  std::getline(std::cin, interfacemode);
  if (interfacemode == "uci") {
    uci::main_loop();
  } else if (interfacemode == "bench") {
    bench::run();
    return 0;
  } else if (interfacemode == "3check") {

    auto starttime = std::chrono::system_clock::now();
    libchess::Position pos{"startpos"};
    std::chrono::milliseconds time{15000};
    auto moves = pos.legal_moves();
    while (!pos.is_terminal()) {
      moves.clear();
      pos.legal_captures(moves);
      pos.legal_noncaptures(moves);
      const auto len = moves.size();
      pos.makemove(moves[rand() % len]);
    }
    auto endtime = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
                     endtime - starttime)
                     .count()
              << "ms to make game of " << pos.history().size() << " moves\n";
    starttime = std::chrono::system_clock::now();
    auto terminal = search::isterminal(pos, Options(0));
    endtime = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
                     endtime - starttime)
                     .count()
              << "ms to check if terminal game over " << terminal << "\n";
    std::cout << pos.history().size() << std::endl;
    pos.undomove();
    std::cout << pos.history().size() << std::endl;
    starttime = std::chrono::system_clock::now();
    auto cc = evaluation::count_checks(pos, Options(1));
    terminal = search::isterminal(pos, Options(1));
    endtime = std::chrono::system_clock::now();
    std::cout << "\n"
              << std::chrono::duration_cast<std::chrono::microseconds>(
                     endtime - starttime)
                     .count()
              << "µs to check for checks. cc=(" << +cc.first << "; " << +cc.second
              << ")" << terminal << "\n";
  } else {
    return 1;
  }
  return 0;
}