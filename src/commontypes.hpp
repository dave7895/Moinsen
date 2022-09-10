#ifndef COMMON_HPP
#define COMMON_HPP
#include <utility>

enum Variants { chess = 0, threecheck, kingofthehill };

struct Options {
  Variants var;
  std::pair<unsigned char, unsigned char> counts;
  int move;
  Options(int i = 0,
          std::pair<unsigned char, unsigned char> cs =
              std::pair<unsigned char, unsigned char>{0, 0},
          int mv = 0)
      : var(Variants(i)), counts(cs), move(mv){};
  Options(Variants v) : var(v){};
};
#endif