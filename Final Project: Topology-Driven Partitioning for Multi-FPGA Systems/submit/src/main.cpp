#include "GlobalTimer.hpp"
#include "Report.h"
#include "Parser.hpp"
#include "Solver.hpp"
#include "IOStreamCreator.hpp"
#include <iostream>
using namespace FPGA_BASE;

namespace {
    std::unique_ptr<FPGA_BASE::Input>
    readInput(int argc, char **argv) {
      Parser Parser;
      auto InputStreamPtr =
          InputStreamCreator().createInputStream(argc, argv);
      auto Input = Parser.parser(*InputStreamPtr);
      Parser.clear();
      return Input;
    }

    void writeOutput(FPGA_Solver &Solver, int argc, char **argv) {
      auto OutputStreamPtr =
          OutputStreamCreator().createOutputStream(argc, argv);
      Solver.getDesignMgr().to_ostream(*OutputStreamPtr);
    }
}

int main(int argc, char **argv) {
  GlobalTimer::initialTimerAndSetTimeLimit(std::chrono::seconds(10 * 60));
  //setting random seed
  srand(seed);

  auto Input = readInput(argc, argv);
  // //Report
  // FPGA_Report report(Input.get());
  // report.report();

  //Solver
  FPGA_Solver Solver(Input.get());
  Solver.solve();

  writeOutput(Solver, argc, argv);

  auto Timer = GlobalTimer::getInstance();
  std::cerr << Timer->getDuration<>().count() / 1e9 << " seconds\n";
  if (Timer->overTime()) {
    std::cerr << "overtime!!\n";
  }
  return 0;
}