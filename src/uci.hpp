#ifndef UCI_HPP
#define UCI_HPP
#include "search.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

namespace uci {
// declarations
void ready() { std::cout << "readyok" << std::endl; }
void position(std::stringstream &strstr, libchess::Position &pos);
void ucinewgame(libchess::Position &pos);
void moves(std::stringstream &sstr, libchess::Position &pos);
void go(std::stringstream &sstr, libchess::Position &pos);
void stop();

std::atomic_bool stopv = false;
std::thread worker;

void main_loop() {
  std::cout << "id name Moinsen\n";
  std::cout << "id author dave7895\n";
  std::cout << "uciok\n";
  for (;;) {
    std::string input;
    std::getline(std::cin, input);
    if (input == "isready") {
      std::cout << "readyok\n";
      break;
    } else if (input == "quit") {
      return;
    }
  }
  libchess::Position pos;
  for (;;) {
    std::string input;
    std::getline(std::cin, input);
    /*std::cout << input << std::endl;
    std::cout << pos.get_fen() << std::endl;*/
    std::stringstream strstr(input);
    std::string word;
    strstr >> word;
    if (word == "quit") {
      stop();
      return;
    } else if (word == "isready") {
      ready();
      if (!(~pos.empty()).count()) {
        ucinewgame(pos);
      }
    } else if (word == "ucinewgame") {
      ucinewgame(pos);
    } else if (word == "position") {
      position(strstr, pos);
    } else if (word == "go") {
      go(strstr, pos);
    } else if (word == "stop") {
      stop();
    }
  }
}

void ucinewgame(libchess::Position &pos) { pos.set_fen("startpos"); }

void position(std::stringstream &strstr, libchess::Position &pos) {
  std::string fen;
  while (!strstr.eof()) {
    std::string word;
    strstr >> word;
    if (word == "moves") {
      pos.set_fen(fen);
      moves(strstr, pos);
      return;
    } else if (word == "fen") {
      continue;
    } else if (fen.empty()) {
      fen = word;
    } else {
      fen += " " + word;
    }
  }
  pos.set_fen(fen);
}

void moves(std::stringstream &sstr, libchess::Position &pos) {
  while (!sstr.eof()) {
    std::string movestr;
    sstr >> movestr;
    pos.makemove(movestr);
  }
}

void go(std::stringstream &sstr, libchess::Position &pos) {
  stop();
  using namespace std::chrono;
  auto time = std::chrono::milliseconds(500);
  bool timeFixed = false;
  int ms_available = 0;
  int inc_ms = 0;
  bool depthFixed = false;
  int depth;
  auto us = pos.turn();
  std::string us_string = us == libchess::White ? "w" : "b";
  while (!sstr.eof()) {
    std::string word;
    sstr >> word;
    if (word == us_string + "time") {
      sstr >> word;
      ms_available = std::stoi(word);
    } else if (word == us_string + "inc") {
      sstr >> word;
      inc_ms = std::stoi(word);
    } else if (word == "depth") {
      sstr >> word;
      depthFixed = true;
      depth = std::stoi(word);
    } else if (word == "movetime") {
      sstr >> word;
      timeFixed = true;
      time = std::chrono::milliseconds(std::stoi(word));
    }
  }
  if (!timeFixed) {
    time = milliseconds(ms_available / 70);
    if (inc_ms > time.count()) {
      time += milliseconds(inc_ms * 7 / 10);
    }
    time = (time * 9) / 10;
    uint64_t available_movec = pos.count_moves() / 5;
    if (available_movec > 0)
      time /= available_movec;
  }
  auto start = system_clock::now();
  if (depthFixed) {
    libchess::Move retMove;
    time = hours(1);
    search::negamax(pos, depth, 1, retMove, stopv, start + time);
    std::cout << "bestmove " << retMove << std::endl;
  } else {
    std::cout << "info string allocated " << time.count() << "ms\n";
    libchess::Move retMove;
    worker = std::thread(search::iterative_deepening, std::ref(pos),
                         start + time, std::ref(stopv));
  }
}

void stop() {
  stopv = true;
  if (worker.joinable()) {
    worker.join();
  }
  stopv = false;
}
} // namespace uci
#endif