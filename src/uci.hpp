#ifndef UCI_HPP
#define UCI_HPP
#include "search.hpp"
#include "commontypes.hpp"
#include <sstream>
#include <string>
#include <thread>

namespace uci {
// declarations
void ready() { std::cout << "readyok" << std::endl; }
void position(std::stringstream &strstr, libchess::Position &pos);
void ucinewgame(libchess::Position &pos);
void moves(std::stringstream &sstr, libchess::Position &pos);
void go(std::stringstream &sstr, libchess::Position &pos, const Options &opts);
void stop();
void setoptions(std::stringstream &sstr, Options &opts);

std::atomic_bool stopv = false;
std::thread worker;

void main_loop() {
  std::cout << "id name Moinsen\n";
  std::cout << "id author dave7895\n";

  std::cout << "\n";
  std::cout << "option name UCI_Variant type combo default chess var 3check var chess var kingofthehill\n";

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
  Options opts;
  for (;;) {
    std::string input;
    std::getline(std::cin, input);
    std::cout << input << std::endl;
    std::cout << pos.get_fen() << std::endl;
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
      opts.counts = evaluation::count_checks(pos, opts);
      opts.move = static_cast<int>(pos.history().size());
      go(strstr, pos, opts);
    } else if (word == "stop") {
      stop();
    } else if (word == "setoption"){
      setoptions(strstr, opts);
    }
  }
}

void ucinewgame(libchess::Position &pos) {
  pos.clear();
  pos.set_fen("startpos");
}

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

void go(std::stringstream &sstr, libchess::Position &pos, const Options &opts) {
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
    if (inc_ms * 2 > time.count()) {
      time += milliseconds(inc_ms * 7 / 10);
    }
    time = (time * 9) / 10;
    if (time.count() > ((ms_available * 2) / 3)) {
      time = milliseconds(ms_available / 5);
    }
  }
  if (depthFixed) {
    auto start = system_clock::now();
    libchess::Move retMove;
    time = hours(1);
    info::searchInfo sInfo;
    int score =
        search::negamax(pos, depth, 1, retMove, stopv, start + time, sInfo, opts);
    const std::chrono::milliseconds elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - start);
    info::infopr(depth, score, retMove, elapsed.count(), sInfo);
    std::cout << "bestmove " << retMove << std::endl;
  } else {
    std::cout << "info string allocated " << time.count() << "ms\n";
    libchess::Move retMove;
    const auto mintime = time;
    worker = std::thread(search::iterative_deepening, std::ref(pos), mintime,
                         std::ref(stopv), opts);
  }
}

void stop() {
  stopv = true;
  if (worker.joinable()) {
    worker.join();
  }
  stopv = false;
}

void setoptions(std::stringstream &sstr, Options &opts){
  std::string placeholder;
	std::string name;
	std::string value;
	sstr >> placeholder >> name >> placeholder >> value;
  if (name.empty() || value.empty()){
    return;
  }
  if (name == "UCI_Variant"){
    if (value == "chess"){
      opts.var = chess;
    } else if (value == "3check"){
      opts.var = threecheck;
    } else if (value == "kingofthehill"){
      opts.var = threecheck;
    }
  }
}

} // namespace uci
#endif