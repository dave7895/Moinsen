#include "eval.hpp"
#include "phasedScore.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <ctime>
#include <execution>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <libchess/position.hpp>
#include <string>
#include <utility>
#include <vector>

using fen_pair = std::pair<std::string, float>;
using pos_pair = std::pair<libchess::Position, float>;

std::vector<fen_pair> readDatafile(const std::string &filepath) {
  std::ifstream infile(filepath, std::ios::in);
  std::vector<fen_pair> data;

  while (infile) {
    float result;
    std::string fen;
    infile >> result;
    infile.get();
    std::getline(infile, fen);
    if (fen.empty()) {
      continue;
    }
    const fen_pair entry{fen, result};
    data.push_back(entry);
    if (infile.eof()) {
      break;
    }
  }
  return data;
}

constexpr std::array<PhaseScore, 64 * 6> init_table() {
  std::array<PhaseScore, 64 * 6> compound_table;
  std::transform(mg_table.begin(), mg_table.end(), eg_table.begin(),
                 compound_table.begin(), [](const int a, const int b) {
                   return S(static_cast<int16_t>(a), static_cast<int16_t>(b));
                 });
  return compound_table;
}

const std::array<PhaseScore, 64 * 6> compound_table = init_table();

float sigmoid(const float x) { return 1 / (1 + expf(-x)); }

double sigmoid(const double x) { return 1 / (1 + exp(-x)); }

double sigmoid(int x) { return sigmoid(static_cast<double>(x)); }

double averageError(const std::vector<pos_pair> &posAndRes,
                    const std::array<int, 64 * 6> &parArray,
                    const std::array<PhaseScore, 6> &pVal) {
  const double trErr = std::transform_reduce(
      std::execution::par, posAndRes.begin(), posAndRes.end(), 0.0,
      std::plus<>(),
      [&parArray, &pVal](const std::pair<libchess::Position, float> &p) {
        const auto pos = p.first;
        const auto res = pos.turn() == libchess::White
                             ? static_cast<double>(p.second)
                             : 1.0 - static_cast<double>(p.second);
        const double thisErr = std::pow(
            static_cast<double>(res) - sigmoid(evaluation::evaluate(
                                           pos, 0, Options(0), parArray, pVal)),
            2.0);
        return thisErr;
      });
  return trErr / static_cast<double>(posAndRes.size());
}

constexpr int16_t supposedPawnVal{100};

void writeTablesToFile(const std::string &path,
                       const std::array<PhaseScore, 384> &optPars,
                       const std::array<PhaseScore, 6> &pVal) {
  const int16_t pawnValue = static_cast<int>(mgScore(pVal[0]));
  std::ofstream os{path};
  const std::array<std::string, 2> startStr = {{
      "constexpr std::array<int16_t, 64*6> mg_table = {{",
      "constexpr std::array<int16_t, 64*6> eg_table = {{",
  }};
  os << "#include <array>\n#include <cstdint>" << std::endl;
  os << "// clang-format off" << std::endl << std::endl;
  for (int phase = 0; phase < 2; phase++) {
    os << startStr[phase] << std::endl;
    int counter = 0;
    int linecounter = 0;
    for (int i = 0; i < 384; i++) {
      const auto val = phase ? egScore(optPars[i]) : mgScore(optPars[i]);
      os << std::setw(3) << ((val * supposedPawnVal) / pawnValue);
      os << ", ";
      counter++;
      if (counter == 8) {
        os << "\n";
        counter = 0;
        linecounter++;
        if (linecounter == 8) {
          os << "\n";
          linecounter = 0;
        }
      }
    }
    os << "}};\n" << std::endl;
  }
  os << "// clang-format on\n" << std::endl;
  os << "constexpr std::array<int, 6> pieceVal = {{";
  for (int i = 0; i < 6; i++) {
    os << "S(";
    os << mgScore(pVal[i]) * supposedPawnVal / pawnValue;
    os << ", ";
    os << egScore(pVal[i]) * supposedPawnVal / pawnValue;
    os << "), ";
  }
  os << "}};\n";
}

std::array<int, 64 * 6> localOptimize(
    const std::vector<std::pair<libchess::Position, float>> &posAndRes,
    const std::array<int, 64 * 6> &initialGuess,
    const std::array<int, 6> &initialpVal) {
  const int nParams = static_cast<int>(initialGuess.size());
  double bestE = averageError(posAndRes, initialGuess, initialpVal);
  std::array<int, 64 * 6> bestParValues = initialGuess;
  auto bestpVal = initialpVal;
  bool improved = true;
  std::cout << "start of optimization" << std::endl;
  int runN = 0;
  while (improved) {
    improved = false;
    runN++;
    const auto timenow =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "starting run number " << runN << " at " << ctime(&timenow);
    std::cout << "lowest error so far at " << bestE;
    std::cout << std::endl;
    for (int pi = 8; pi < nParams; pi++) {
      for (int j = 0; j < 2; j++) {
        const int plusConst = j ? S(1, 0) : S(0, 1);
        std::array<int, 64 * 6> newParValues = bestParValues;
        newParValues[pi] += plusConst;
        double newE = averageError(posAndRes, newParValues, bestpVal);
        if (newE < bestE) {
          bestE = newE;
          bestParValues = newParValues;
          improved = true;
        } else {
          newParValues[pi] -= 2 * plusConst;
          newE = averageError(posAndRes, newParValues, bestpVal);
          if (newE < bestE) {
            bestE = newE;
            bestParValues = newParValues;
            improved = true;
          }
        }
      }
    }

    for (int pi = 0; pi < 6; pi++) {
      for (int j = 0; j < 2; j++) {
        const int plusConst = j ? S(1, 0) : S(0, 1);
        auto newpVal = bestpVal;
        newpVal[pi] += plusConst;
        double newE = averageError(posAndRes, bestParValues, newpVal);
        if (newE < bestE) {
          bestE = newE;
          bestpVal = newpVal;
          improved = true;
        } else {
          newpVal[pi] -= 2 * plusConst;
          newE = averageError(posAndRes, bestParValues, newpVal);
          if (newE < bestE) {
            bestE = newE;
            bestpVal = newpVal;
            improved = true;
          }
        }
      }
    }
    writeTablesToFile("correctTuning.hpp", bestParValues, bestpVal);
  }
  return bestParValues;
}

int main(int argc, char const *argv[]) {
  auto opt = compound_table;
  const auto optpVal = evaluation::pieceVal;
  if (argc > 1) {
    std::string path{argv[1]};
    std::vector<fen_pair> fenData = readDatafile(path);
    fenData.shrink_to_fit();
    std::vector<std::pair<libchess::Position, float>> posAndRes;
    for (const auto &[fen, result] : fenData) {
      if (fen.empty()) {
        continue;
      }
      const libchess::Position pos{fen};
      std::pair<libchess::Position, float> pR{pos, result};
      posAndRes.push_back(pR);
    }
    posAndRes.shrink_to_fit();
    std::cout << "file has " << fenData.size() << " entries\n";
    double err = averageError(posAndRes, compound_table, optpVal);
    std::cout << "average Error is " << err << std::endl;
    opt = localOptimize(posAndRes, compound_table, optpVal);
    err = averageError(posAndRes, opt, optpVal);
    std::cout << "average Error is " << err << std::endl;
  }
  return 0;
}
