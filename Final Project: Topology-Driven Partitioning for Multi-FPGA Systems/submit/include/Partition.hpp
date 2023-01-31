#pragma once
#include <iostream>
#include <bitset>
#include <algorithm>
#include <string>
#include <memory>
#include <cstdlib>
#include <queue>
#include <math.h>
#include "Data.hpp"
#include "Input.hpp"
#include "DesignManager.hpp"

namespace FPGA_BASE {

	class FPGA_Partition {
		public:
			FPGA_Partition(FPGA_DesignMgr &DesignMgr, FPGA_AfterPropagate *AfterPropagate) :
				m_DesignMgr(DesignMgr),
                m_node_candidate(AfterPropagate->get_node_candidate()),
                m_reachable_FPGAs(AfterPropagate->get_reachable_FPGAs()),
                m_fixed_node(AfterPropagate->get_fixed_node())
				{}
			void start_partition();
            void movable_node_placement();
            void no_cddt_node_placement();
            void update_movable_node_fpga_candidate(FPGA_Node* node, BITSET &final_cand);
            void create_fpga();
            void set_node_on_fpga(FPGA_Node* node, FPGA_fpga* fpga);
            int get_external_degree(FPGA_Node* node, FPGA_fpga* fpga);
            void fixed_node_placement();
            void neighbor_of_fixed_node_placement();
            void sort_node_fpga_candidate(std::vector<std::pair<int, int>> &node_fpga_candidate, FPGA_Node *node);
            void add_candidate_info2node();
            void update_fpga_candidate();
            void set_full_fpga_candidate(FPGA_fpga* fpga);
            void add_fpga_candidate(int fpga_id);
            void delete_fpga_candidate(int fpga_id);
            void report_fpga_candidate() const;

            struct node_candidate_cmp {
                bool operator() (FPGA_Node* a, FPGA_Node* b) {return a->get_candidate().count() < b->get_candidate().count();}
            } node_candidate_cmp;

            struct node_fpga_candidate_cmp { //<fpga id, fpga candidate>
                bool operator() (std::pair<int, int> &a, std::pair<int, int> &b) {return a.second < b.second;}
            } node_fpga_candidate_cmp;

            struct no_cddt_node_placement_cmp { //<fpga id, fpga candidate>
                bool operator() (std::pair<int, int> &a, std::pair<int, int> &b) {return a.second > b.second;}
            } no_cddt_node_placement_cmp;
            

		private:
			FPGA_DesignMgr &m_DesignMgr;
            std::vector<BITSET> &m_node_candidate;
            std::vector<std::vector<BITSET>> &m_reachable_FPGAs;
            std::vector<int> &m_fixed_node;
            std::vector<FPGA_fpga*> m_fpga; //from 0 ~ fpga size
            std::map<int, FPGA_fpga*> m_fpga_map; // <fpga id, fpga> from 0 ~ fpga size
            std::vector<std::pair<FPGA_fpga*, int>> m_fpga_candidate; // <fpga, fpga candidate num> from 0 ~ fpga size
	};

    inline void FPGA_Partition::report_fpga_candidate() const{
        assert(!m_fpga_candidate.empty());
        std::cout << "====fpga_candidate====\n";
        for(auto &fpga_candidate : m_fpga_candidate){
            std::cout << "fpga id: " << fpga_candidate.first->get_fpga_id() << " candidate count: " << fpga_candidate.second << '\n';
        }
    }

} //FPGA_BASE