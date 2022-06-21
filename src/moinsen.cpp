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
  } else {
    return 1;
  }
  return 0;
}