#pragma once
#include <iostream>
#include <bitset>
#include <algorithm>
#include <string>
#include "Data.hpp"
#include "Input.hpp"
#include "DesignManager.hpp"

namespace FPGA_BASE {

	class FPGA_Propagation {
		public:
			FPGA_Propagation(FPGA_DesignMgr &DesignMgr) :
				m_DesignMgr(DesignMgr)
				{}
			std::unique_ptr<FPGA_AfterPropagate> start_propagate();
		private:
			FPGA_DesignMgr &m_DesignMgr;
			std::vector<BITSET> Ready_Node_Candidate();
			std::vector<std::vector<BITSET>> Ready_Reachable_FPGAs();
			std::vector<int> Find_Fixed_Nodes();
			void Set_Fixed_Node_Candidate(
				std::vector<BITSET> &node_candidate_SFNC, 
				std::vector<int> &fixed_node_SFNC);
			void Unfixed_Node_Propagation(
				std::vector<BITSET> &node_candidate_UFNP, 
				std::vector<std::vector<BITSET>> &reachable_FPGAs_UFNP, 
				std::vector<int> &fixed_node_UFNP);
			void Report_Node_Candidate(std::vector<BITSET> &node_candidate_R);
			void Report_FPGA_Candidate(std::vector<std::vector<BITSET>> &FPGA_candidate_R);

	};

} //FPGA_BASE