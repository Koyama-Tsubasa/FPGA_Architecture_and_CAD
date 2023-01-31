#pragma once
#include <iostream>
#include <bitset>
#include <ostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <assert.h>
#define seed 10
typedef std::bitset<100> BITSET;

namespace FPGA_BASE {
    // < Total number of FPGAs> <Total number of FPGA connection channels> 
    // <Capacity per FPGA> <Total number of nodes>
    // <Total number of nets> <Total number of fixed nodes>
    class FPGA_Parameter;
    class FPGA_Channel;
    class FPGA_fpga;
    class FPGA_Node;
    class FPGA_Net;

    class FPGA_Parameter {
        public:
            FPGA_Parameter() :
                m_fpga_nums(0),
                m_fpga_channel_nums(0),
                m_fpga_cap(0),
                m_total_nodes(0),
                m_total_nets(0),
                m_total_fixed_nodes(0)
                {}
            FPGA_Parameter(int fpga_nums, int fpga_channel_nums, int fpga_cap, int total_nodes, int total_nets, int total_fixed_nodes) : 
                m_fpga_nums(fpga_nums),
                m_fpga_channel_nums(fpga_channel_nums),
                m_fpga_cap(fpga_cap),
                m_total_nodes(total_nodes),
                m_total_nets(total_nets),
                m_total_fixed_nodes(total_fixed_nodes)
                {}
            void set_fpga_nums(int n) {m_fpga_nums = n;}
            void set_fpga_channel_nums(int n) {m_fpga_channel_nums = n;}
            void set_fpga_cap(int n) {m_fpga_cap = n;}
            void set_total_nodes(int n) {m_total_nodes = n;}
            void set_total_nets(int n) {m_total_nets = n;}
            void set_total_fixed_nodes(int n) {m_total_fixed_nodes = n;}
            int get_fpga_nums() const {return m_fpga_nums;}
            int get_fpga_channel_nums() const {return m_fpga_channel_nums;}
            int get_fpga_cap() const {return m_fpga_cap;}
            int get_total_nodes() const {return m_total_nodes;}
            int get_total_nets() const {return m_total_nets;}
            int get_total_fixed_nodes() const {return m_total_fixed_nodes;}
            void set_fpga_parameter(int fpga_nums, int fpga_channel_nums, int fpga_cap, int total_nodes, int total_nets, int total_fixed_nodes){
                m_fpga_nums = fpga_nums;
                m_fpga_channel_nums = fpga_channel_nums;
                m_fpga_cap = fpga_cap;
                m_total_nodes = total_nodes;
                m_total_nets = total_nets;
                m_total_fixed_nodes = total_fixed_nodes;
            };
            void report() const;
        private:
            int m_fpga_nums;
            int m_fpga_channel_nums;
            int m_fpga_cap;
            int m_total_nodes;
            int m_total_nets;
            int m_total_fixed_nodes;
    };
    
    class FPGA_Channel {
        public:
            FPGA_Channel() :
                m_fpga_id1(0),
                m_fpga_id2(0)
                {}
            FPGA_Channel(int fpga_id1, int fpga_id2) :
                m_fpga_id1(fpga_id1),
                m_fpga_id2(fpga_id2)
                {}
            int get_fpga_id1() const {return m_fpga_id1;}
            int get_fpga_id2() const {return m_fpga_id2;}
            void report() const {
                std::cout << "fpga_id1: " << m_fpga_id1 << " "
                          << "fpga_id2: " << m_fpga_id2 << '\n';
            }
        private:
            int m_fpga_id1;
            int m_fpga_id2;
    };

    class FPGA_fpga {
        public:
            FPGA_fpga(int fpga_id, int cap) :
                m_fpga_id(fpga_id),
                m_cap(cap)
                {}
            void set_fpga_id(int id) {m_fpga_id = id;}
            void set_cap(int cap) {m_cap = cap;}
            void set_all_node(std::vector<FPGA_Node*> &nodes) {m_nodes = nodes;}
            void add_node(FPGA_Node* node) {m_nodes.push_back(node);}
            int get_fpga_id() const {return m_fpga_id;}
            int get_cap() const {return m_cap;}
            int get_remain_cap() const {return m_cap - m_nodes.size();}
            bool is_full() {
                if(m_nodes.empty()){
                    return false;
                } else {
                    if(m_nodes.size() < m_cap){
                        return false;
                    } else {
                        return true;
                    }
                }
            }
            void report() const;
            std::vector<FPGA_Node*> &get_nodes() {return m_nodes;}
        private:
            int m_fpga_id;
            int m_cap;
            std::vector<FPGA_Node*> m_nodes;
    };

    class FPGA_Node {
        public:
            FPGA_Node(int node_id) :
                m_node_id(node_id)
                {};
            struct cmp {
                bool operator()(const FPGA_Node* x, const FPGA_Node* y) const { return x->m_node_id < y->m_node_id; }
            };
            void set_is_fixed(const bool b) {m_is_fixed = b;}
            void set_node_id(const int id) {m_node_id = id;}
            void set_fixed_fpga_id(const int fixed_fpga_id) {m_fixed_fpga_id = fixed_fpga_id;}
            void add_net_id(const int net_id) {m_net_id.push_back(net_id);}
            void add_neighbor_node(FPGA_Node* neighbor_node) {m_neighbor_node.insert(neighbor_node);}
            void set_net_id(std::vector<int> &net_id) {m_net_id = net_id;}
            void set_neighbor_node(std::set<FPGA_Node* , cmp> &neighbor_node) {m_neighbor_node = neighbor_node;}
            void set_is_placed(bool b) {m_is_placed = b;}
            void set_fpga(FPGA_fpga *fpga) {m_fpga = fpga;}
            void set_candidate(BITSET &candidate) {m_candidate = candidate;}
            int get_node_id() const {return m_node_id;}
            bool get_is_fixed() const {return m_is_fixed;}
            int get_fixed_fpga_id() const {return m_fixed_fpga_id;}
            bool get_is_placed() {return m_is_placed;}
            FPGA_fpga *get_fpga() {return m_fpga;}
            std::vector<int> &get_net_id() {return m_net_id;}
            std::set<FPGA_Node*, cmp> &get_neighbor_node() {return m_neighbor_node;}
            BITSET &get_candidate() {return m_candidate;}
            void report() const;
            void report_candidate(int fpga_num) const;
        private:
            int m_node_id;
            bool m_is_fixed = false;
            int m_fixed_fpga_id = 0;
            bool m_is_placed = false;
            BITSET m_candidate;
            FPGA_fpga *m_fpga = nullptr;
            std::vector<int> m_net_id;
            std::set<FPGA_Node*, cmp> m_neighbor_node;
    };

    class FPGA_Net {
        public:
            FPGA_Net(int net_id) :
                m_net_id(net_id)
                {};
            void set_net_id(const int net_id) {m_net_id = net_id;}
            void set_source_node(FPGA_Node *source_node) {m_source_node = source_node;}
            void add_sink_node(FPGA_Node *sink_node) {m_sink_node.push_back(sink_node);}
            void set_sink_node(std::vector<FPGA_Node*> &sink_node) {m_sink_node = sink_node;}
            int get_net_id() const {return m_net_id;}
            FPGA_Node *get_source_node() {return m_source_node;}
            std::vector<FPGA_Node*> &get_sink_node() {return m_sink_node;}
            std::vector<FPGA_Node*> get_all_nodes() {
                std::vector<FPGA_Node*> all_nodes;
                all_nodes.push_back(m_source_node);
                for(auto &sink : m_sink_node){
                    all_nodes.push_back(sink);
                }
                return all_nodes;
            }
            void report() const;
        private:
            int m_net_id;
            FPGA_Node *m_source_node;
            std::vector<FPGA_Node*> m_sink_node;
    };

    inline void FPGA_Parameter::report() const {
        std::cout << "Total number of FPGAs: " << get_fpga_nums() << " " 
                    << "Total number of FPGA connection channels: " << get_fpga_channel_nums() << " "
                    << "Capacity per FPGA: " << get_fpga_cap() << "\n"
                    << "Total number of nodes: " << get_total_nodes() << " "
                    << "Total number of nets: " << get_total_nets() << " "
                    << "Total number of fixed nodes: " << get_total_fixed_nodes() << "\n\n";
    }

    inline void FPGA_fpga::report() const {
        std::cout << "fpga id: " << m_fpga_id << " " << "cap: " << m_cap << " " << "remain cap: " << get_remain_cap() << '\n';
        if(!m_nodes.empty()){
            for(auto node : m_nodes){
                if(node != nullptr){
                    node->report();
                }
            }
        }
    }

    inline void FPGA_Node::report() const {
        std::cout << "Node id: " << m_node_id << '\n';
        std::cout << "is_fixed: " << m_is_fixed << " "
                  << "fixed_fpga_id: " << m_fixed_fpga_id << '\n';

        std::cout << "is_placed: " << m_is_placed << '\n';
        if(m_is_placed && m_fpga != nullptr){
                    std::cout << "fpga_id: " << m_fpga->get_fpga_id() << '\n';
        }
        
        std::cout << "Net id: ";
        for(auto &iter : m_net_id){
            std::cout << iter << " ";
        }
        std::cout << '\n';

        std::cout << "Neighbor_node id: ";
        for(auto &iter : m_neighbor_node){
            std::cout << iter->get_node_id() << " ";
        }
        std::cout << "\n\n";
    }

    inline void FPGA_Node::report_candidate(int fpga_num) const {
        auto bitstring = m_candidate.to_string();
		bitstring = bitstring.substr(m_candidate.size()-fpga_num, m_candidate.size());
		reverse(bitstring.begin(), bitstring.end());
		std::cout << "node id " << m_node_id << ": " << bitstring << std::endl;
    }

    inline void FPGA_Net::report() const {
        std::cout << "Net id: " << m_net_id << '\n';
        std::cout << "++++++\n";
        std::cout << "Source_node id: \n";
        m_source_node->report();
        std::cout << "++++++\n";
        std::cout << "Sink_node id:\n";
        for(auto &iter : m_sink_node){
            iter->report();
            std::cout << "------\n";
        }
        std::cout << "\n\n";
    }

}