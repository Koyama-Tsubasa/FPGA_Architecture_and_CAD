#pragma once
#include <iostream>
#include <memory>
#include <bitset>
#include "Data.hpp"
#include "Input.hpp"

namespace FPGA_BASE {

	class FPGA_DesignMgr {
		public:
			FPGA_DesignMgr(Input *InputPtr);
            FPGA_Parameter &get_fpga_parameter() {return m_fpga_parameter;}
            std::vector<FPGA_Channel> &get_fpga_channels() {return m_fpga_channels;}
            std::vector<FPGA_Net*> &get_fpga_nets() {return m_fpga_nets;}
            std::vector<FPGA_Node*> &get_fpga_nodes() {return m_fpga_nodes;}
            std::map<std::string, FPGA_Node*> &get_NodeMap() {return m_NodeMap;}
            std::map<int, FPGA_Net*> &get_NetMap() {return m_NetMap;}
            void to_ostream(std::ostream &out) const;
		private:
			Input *m_InputPtr;
            FPGA_Parameter m_fpga_parameter;
            std::vector<FPGA_Channel> m_fpga_channels;
            std::vector<FPGA_Net*> m_fpga_nets;
            std::vector<FPGA_Node*> m_fpga_nodes;
            std::map<std::string, FPGA_Node*> m_NodeMap;
            std::map<int, FPGA_Net*> m_NetMap;
	};

    class FPGA_AfterPropagate {
        public:
            FPGA_AfterPropagate(std::vector<BITSET> &node_candidate, 
	                            std::vector<std::vector<BITSET>> &reachable_FPGAs,
	                            std::vector<int> &fixed_node) :
                m_node_candidate(node_candidate),
                m_reachable_FPGAs(reachable_FPGAs),
                m_fixed_node(fixed_node)
                {}
            std::vector<BITSET> &get_node_candidate() {return m_node_candidate;}
            std::vector<std::vector<BITSET>> &get_reachable_FPGAs() {return m_reachable_FPGAs;}
            std::vector<int> &get_fixed_node() {return m_fixed_node;}
        private:
            std::vector<BITSET> m_node_candidate;
	        std::vector<std::vector<BITSET>> m_reachable_FPGAs;
	        std::vector<int> m_fixed_node;
    };

} //FPGA_BASE