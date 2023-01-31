#include "../include/Propagation.hpp"

namespace FPGA_BASE {

std::unique_ptr<FPGA_AfterPropagate> FPGA_Propagation::start_propagate(){

	auto node_candidate = Ready_Node_Candidate();
	auto reachable_FPGAs = Ready_Reachable_FPGAs();
	auto fixed_node = Find_Fixed_Nodes();
	Set_Fixed_Node_Candidate(node_candidate, fixed_node);
	Unfixed_Node_Propagation(node_candidate, reachable_FPGAs, fixed_node);
	return std::unique_ptr<FPGA_AfterPropagate>(new FPGA_AfterPropagate(node_candidate, reachable_FPGAs, fixed_node));
}

std::vector<BITSET> FPGA_Propagation::Ready_Node_Candidate() {

	int fpga_num = m_DesignMgr.get_fpga_parameter().get_fpga_nums();
	int node_num = m_DesignMgr.get_fpga_parameter().get_total_nodes();
	std::vector<BITSET> prepare_node_candidate;
	prepare_node_candidate.reserve(node_num);
	for (int i=0; i<node_num; i++) {

		BITSET initial_candidate;
		prepare_node_candidate.push_back(initial_candidate.set());

	}

	// output bitsets
	std::cout << "\n----------------------------------" << std::endl;
	std::cout << "FPGA node candidate initialization" << std::endl;
	std::cout << "----------------------------------\n" << std::endl;
	// Report_Node_Candidate(prepare_node_candidate);

	return prepare_node_candidate;

}

std::vector<std::vector<BITSET>> FPGA_Propagation::Ready_Reachable_FPGAs() {

	int fpga_num = m_DesignMgr.get_fpga_parameter().get_fpga_nums();
	int fpga_channel_num = m_DesignMgr.get_fpga_parameter().get_fpga_channel_nums();
	auto fpga_channels = m_DesignMgr.get_fpga_channels();
	BITSET fpga_bits;
	std::vector<std::vector<BITSET>> p_reachable_fpgas(fpga_num);
	
	// initialize dist 1 for all FPGAs
	for (int i=0; i<fpga_num; i++) 
		p_reachable_fpgas[i].push_back(fpga_bits);

	// calculate dist 1 for all FPGAs
	for (int i=0; i<fpga_channel_num; i++) {

		int con_1 = fpga_channels[i].get_fpga_id1();
		int con_2 = fpga_channels[i].get_fpga_id2();

		// for connection 1
		p_reachable_fpgas[con_1][0].set(con_1);
		p_reachable_fpgas[con_1][0].set(con_2);

		// for connection 2
		p_reachable_fpgas[con_2][0].set(con_1);
		p_reachable_fpgas[con_2][0].set(con_2);

	}

	// calculate all distance for all FPGAs
	for (int i=0; i<fpga_num; i++) {

		// std::cout << "FPGA: " << i << std::endl;
		int dis = 0;
		
		while (p_reachable_fpgas[i][dis].count() != fpga_num) {
			
			BITSET additions;
			p_reachable_fpgas[i].push_back(fpga_bits);
		
			// find all 1s in bitset
			for (int j=p_reachable_fpgas[i][dis]._Find_first(); 
					 j<fpga_bits.size(); 
					 j=p_reachable_fpgas[i][dis]._Find_next(j)) {

				// std::cout << "add FPGA " << j << ": " << p_reachable_fpgas[j][0] << std::endl;
				additions |= p_reachable_fpgas[j][0];

			}

			// update the reachable FPGAs this distance
			p_reachable_fpgas[i][++dis] |= additions;

		}

	}

	// output reachable fpgas
	std::cout << "\n------------------------------------" << std::endl;
	std::cout << "FPGA reachable candidate calculation" << std::endl;
	std::cout << "------------------------------------\n" << std::endl;
	//Report_FPGA_Candidate(p_reachable_fpgas);

	return p_reachable_fpgas;

}

std::vector<int> FPGA_Propagation::Find_Fixed_Nodes() {

	std::vector<int> fixed_nodes;
	int node_num = m_DesignMgr.get_fpga_parameter().get_total_nodes();
	auto fpga_nodes = m_DesignMgr.get_fpga_nodes();

	// find fixed node by iterating all nodes
	for (int i=0; i<node_num; i++) 
		if (fpga_nodes[i]->get_is_fixed())
			fixed_nodes.push_back(fpga_nodes[i]->get_node_id());

	// output fixed nodes
	// std::cout << "\n----------------" << std::endl;
	// std::cout << "find fixed nodes" << std::endl;
	// std::cout << "----------------\n" << std::endl;
	// for (int i=0; i<fixed_nodes.size(); i++)
	// 	std::cout << fixed_nodes[i] << " ";
	// std::cout << std::endl;

	return fixed_nodes;

}

void FPGA_Propagation::Set_Fixed_Node_Candidate(
	std::vector<BITSET> &node_candidate_SFNC, 
	std::vector<int> &fixed_node_SFNC) {

	auto fpga_nodes = m_DesignMgr.get_fpga_nodes();
	for (auto node_id: fixed_node_SFNC) {

		int fixed_fpga_id = fpga_nodes[node_id]->get_fixed_fpga_id();
		node_candidate_SFNC[node_id].reset();
		node_candidate_SFNC[node_id].set(fixed_fpga_id);

	}

}

void FPGA_Propagation::Unfixed_Node_Propagation(
	std::vector<BITSET> &node_candidate_UFNP, 
	std::vector<std::vector<BITSET>> &reachable_FPGAs_UFNP,
	std::vector<int> &fixed_node_UFNP) {

	auto fpga_nodes = m_DesignMgr.get_fpga_nodes();

	// for each fixed node
	for (auto fixed_node_id: fixed_node_UFNP) {
		
		auto distances_candidates = reachable_FPGAs_UFNP[fpga_nodes[fixed_node_id]->get_fixed_fpga_id()];
		std::vector<int> current_neighbors;
		std::set<int> next_neighbors;
		current_neighbors.push_back(fixed_node_id);

		// for each distance except the longest one
		for (int dis=0; dis<distances_candidates.size()-1; dis++) {

			// find all neighbors from current neighbors
			for (auto current_neighbor: current_neighbors) 
				for (auto next_neighbor: fpga_nodes[current_neighbor]->get_neighbor_node()) 
					next_neighbors.insert(next_neighbor->get_node_id());
			
			// intersection between node candidate & distance candidate
			for (auto neighbor: next_neighbors) 
				node_candidate_UFNP[neighbor] &= distances_candidates[dis];

			// update current neighbors and next neighbors
			current_neighbors.clear();
			current_neighbors.assign(next_neighbors.begin(), next_neighbors.end());
			next_neighbors.clear();

		}
		
	}

	// output updated candidates of all nodes
	std::cout << "\n---------------------------------------------" << std::endl;
	std::cout << "candidate of all nodes after FPGA Propagation" << std::endl;
	std::cout << "---------------------------------------------\n" << std::endl;
	// Report_Node_Candidate(node_candidate_UFNP);
	
}

void FPGA_Propagation::Report_Node_Candidate(std::vector<BITSET> &node_candidate_R) {

	int fpga_num = m_DesignMgr.get_fpga_parameter().get_fpga_nums();
	int i = 0;
	for (auto node_candidate: node_candidate_R) {

		auto bitstring = node_candidate.to_string();
		bitstring = bitstring.substr(node_candidate.size()-fpga_num, node_candidate.size());
		reverse(bitstring.begin(), bitstring.end());
		std::cout << "node " << i++ << ": " << bitstring << std::endl;

	}

}

void FPGA_Propagation::Report_FPGA_Candidate(std::vector<std::vector<BITSET>> &FPGA_candidate_R) {

	BITSET fpga_bits;
	int fpga_num = m_DesignMgr.get_fpga_parameter().get_fpga_nums();
	for (int i=0; i<fpga_num; i++) {

		std::cout << "FPGA " << i << ":" << std::endl;
		for (int j=0; j<FPGA_candidate_R[i].size(); j++) {

			auto bitstring = FPGA_candidate_R[i][j].to_string().substr(fpga_bits.size()-fpga_num, fpga_bits.size());
			reverse(bitstring.begin(), bitstring.end());
			std:: cout << "dis " << j+1 << ": " << bitstring << std::endl;

		}
		if (i < fpga_num-1) std::cout << std::endl;

	}

}

} //FPGA_BASE
