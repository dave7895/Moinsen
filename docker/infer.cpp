#include "src/commontypes.hpp"
#include "src/search.hpp"
#include <chrono>
#include <cstdlib>
#include <google/cloud/functions/framework.h>
#include <iostream>
#include <sstream>
#include <string>

namespace gcf = ::google::cloud::functions;

std::string extract_std_fen(std::string fen, Options &opts) {
  std::string collect;
  std::string placeholder;
  opts.move = 0;
  std::stringstream sstr{fen};
  while (sstr >> placeholder) {
    if (placeholder[1] != '+') {
      if (!collect.empty()) {
        collect += " ";
      }
      collect += placeholder;
    } else {
      opts.counts.first = static_cast<uint16_t>('3' - placeholder[0]);
      opts.counts.second = '3' - placeholder[2];
    }
  }
  return collect;
}

gcf::HttpResponse infer(gcf::HttpRequest request) {
  std::string fen;
  Options opts;
  int availableTime;
  std::string content = request.payload();
  if (content.empty()) {
    fen = "startpos";
    availableTime = 100;
  } else {
    std::cout << content << std::endl;
    std::string varname;
    std::stringstream sstr{content};
    std::getline(sstr, varname);
    if (varname == "chess") {
      opts.var = chess;
    } else if (varname == "3check" || varname == "threecheck") {
      opts.var = threecheck;
    } else if (varname == "kingofthehill" || varname == "koth") {
      opts.var = threecheck;
    }
    std::getline(sstr, fen);
    sstr >> availableTime;
  }
  fen = extract_std_fen(fen, opts);
  libchess::Position pos{fen};
  const auto chTime = std::chrono::milliseconds(availableTime * 2 / 3);
  int eval = 100;
  std::atomic_bool stop = false;
  libchess::Move move = search::iterative_deepening(pos, chTime, stop, opts);
  std::string moveStr = static_cast<std::string>(move);
  std::string answer = std::to_string(eval) + "\n" + moveStr;
  return gcf::HttpResponse{}
      .set_header("content-type", "text/plain")
      .set_payload(answer);
}

auto make_infer_func() { return gcf::MakeFunction(infer); }

int main(int argc, char *argv[]) {
  return gcf::Run(argc, argv, make_infer_func());
}