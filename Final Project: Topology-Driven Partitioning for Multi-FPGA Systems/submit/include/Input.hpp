#pragma once
#include <ostream>
#include <string>
#include <vector>
#include <map>
#include "Data.hpp"

namespace FPGA_BASE {
    class Input {
        public:
            Input(const FPGA_Parameter &fpga_parameter, const std::vector<FPGA_Channel> &fpga_channels, 
                  const std::vector<FPGA_Net*> &fpga_nets, const std::vector<FPGA_Node*> &fpga_nodes,
                  const std::map<std::string, FPGA_Node*> &NodeMap,
                  const std::map<int, FPGA_Net*> &NetMap) :
                m_fpga_parameter(fpga_parameter),
                m_fpga_channels(fpga_channels),
                m_fpga_nets(fpga_nets),
                m_fpga_nodes(fpga_nodes),
                m_NodeMap(NodeMap),
                m_NetMap(NetMap)
                {}
            const FPGA_Parameter &get_fpga_parameter() const {return m_fpga_parameter;}
            const std::vector<FPGA_Channel> &get_fpga_channels() const {return m_fpga_channels;}
            const std::vector<FPGA_Net*> &get_fpga_nets() const {return m_fpga_nets;}
            const std::vector<FPGA_Node*> &get_fpga_nodes() const {return m_fpga_nodes;}
            const std::map<std::string, FPGA_Node*> &get_NodeMap() const {return m_NodeMap;}
            const std::map<int, FPGA_Net*> &get_NetMap() const {return m_NetMap;}
        private:
            const FPGA_Parameter m_fpga_parameter;
            const std::vector<FPGA_Channel> m_fpga_channels;
            const std::vector<FPGA_Net*> m_fpga_nets;
            const std::vector<FPGA_Node*> m_fpga_nodes;
            const std::map<std::string, FPGA_Node*> m_NodeMap;
            const std::map<int, FPGA_Net*> m_NetMap;
    };
    
}