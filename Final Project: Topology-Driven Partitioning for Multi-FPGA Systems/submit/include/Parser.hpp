#pragma once
#include <istream>
#include <iostream>
#include <sstream>
#include <memory>
#include <tuple>
#include <vector>
#include <map>
#include <string>
#include <string.h>
#include "Input.hpp"
#include "Data.hpp"

namespace FPGA_BASE {
class Parser {
  public:
    std::unique_ptr<FPGA_BASE::Input> parser(std::istream &input);
    void clear();
  private:
  // < Total number of FPGAs> <Total number of FPGA connection channels> 
  // <Capacity per FPGA> <Total number of nodes>
  // <Total number of nets> <Total number of fixed nodes>
    std::tuple<int, int, int, int, int, int> parseFPGAParameters(std::istream &input);
    FPGA_Channel parseFPGAChannel(std::istream &input);
    FPGA_Net* parseFPGANet(std::istream &input);
    void parseFPGAFixedNode(std::istream &input);
    std::vector<FPGA_Node*> m_fpga_nodes;
    std::vector<FPGA_Net*> m_fpga_nets;
    std::map<std::string, FPGA_Node*> NodeMap;
    std::map<int, FPGA_Net*> NetMap;
};
} //FPGA_BASE