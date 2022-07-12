#include "src/search.hpp"
#include <chrono>
#include <cstdlib>
#include <google/cloud/functions/framework.h>
#include <iostream>
#include <sstream>
#include <string>

namespace gcf = ::google::cloud::functions;

gcf::HttpResponse infer(gcf::HttpRequest request) {
  std::string fen;
  int availableTime;
  std::string content = request.payload();
  if (content.empty()) {
    fen = "startpos";
    availableTime = 1000;
  } else {
    std::cout << content << std::endl;
    std::stringstream sstr{content};
    std::getline(sstr, fen);
    sstr >> availableTime;
  }
  libchess::Position pos{fen};
  const auto chTime = std::chrono::milliseconds(availableTime * 2 / 3);
  int eval = 100;
  std::atomic_bool stop = false;
  libchess::Move move = search::iterative_deepening(pos, chTime, stop);
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