#include "Partition.hpp"

namespace FPGA_BASE {

void FPGA_Partition::start_partition() {
    create_fpga();
    add_candidate_info2node();
    update_fpga_candidate();
    std::cout << "\n----------------------------------" << std::endl;
	std::cout << "Fixed Node Placement" << std::endl;
	std::cout << "----------------------------------\n" << std::endl;
    fixed_node_placement();
    std::cout << "\n----------------------------------" << std::endl;
	std::cout << "Neighbor of Fixed Node Placement" << std::endl;
	std::cout << "----------------------------------\n" << std::endl;
    neighbor_of_fixed_node_placement();
    std::cout << "\n----------------------------------" << std::endl;
	std::cout << "Movable Node Placement" << std::endl;
	std::cout << "----------------------------------\n" << std::endl;
    movable_node_placement();
    // no_cddt_node_placement();
}

void FPGA_Partition::create_fpga(){
    int cap = m_DesignMgr.get_fpga_parameter().get_fpga_cap();
    for(int idx = 0; idx < m_DesignMgr.get_fpga_parameter().get_fpga_nums(); idx++){
        FPGA_fpga *fpga = new FPGA_fpga(idx, cap);
        m_fpga.push_back(fpga);
        m_fpga_map[idx] = fpga;
    }
}

void FPGA_Partition::add_candidate_info2node(){
    for(size_t idx  = 0; idx < m_node_candidate.size(); idx++){
        m_DesignMgr.get_NodeMap()[std::to_string(idx)]->set_candidate(m_node_candidate[idx]);
    }
}

void FPGA_Partition::update_fpga_candidate(){
    assert(!m_fpga.empty() && !m_fpga_map.empty());
    m_fpga_candidate.clear();
    m_fpga_candidate.reserve(m_fpga.size());

    std::vector<int> fpga_candidate;
    fpga_candidate.resize(m_fpga.size(), 0); //initial to 0
    for(auto &node : m_DesignMgr.get_fpga_nodes()){
        for(size_t idx = 0; idx < m_fpga.size(); idx++){
            if(node->get_candidate()[idx] == 1){
                fpga_candidate[idx]++;
            }
        }
    }
    for(size_t idx = 0; idx < m_fpga.size(); idx++){
        m_fpga_candidate.push_back(std::make_pair(m_fpga_map[idx], fpga_candidate[idx]));
    }
}

void FPGA_Partition::add_fpga_candidate(int fpga_id){
    m_fpga_candidate[fpga_id].second++;
}

void FPGA_Partition::delete_fpga_candidate(int fpga_id){
    if(m_fpga_candidate[fpga_id].second > 0){
        m_fpga_candidate[fpga_id].second--;
    }
}

void FPGA_Partition::set_full_fpga_candidate(FPGA_fpga* fpga){
    assert(fpga->is_full());
    for(auto &node : m_DesignMgr.get_fpga_nodes()){
        if(node->get_candidate()[fpga->get_fpga_id()] == 1 && 
           !node->get_is_placed()){
            node->get_candidate().reset(fpga->get_fpga_id());
        }
    }
}

void FPGA_Partition::set_node_on_fpga(FPGA_Node* node, FPGA_fpga* fpga) {
    
    assert(fpga != nullptr && !fpga->is_full());
    fpga->add_node(node);
    node->set_fpga(fpga);
    node->set_is_placed(true);
    assert(!m_fpga.empty());
    auto &candidate = node->get_candidate();
    for(int idx = 0; idx < m_fpga.size(); idx++){
        if(candidate[idx] == 1 && idx != fpga->get_fpga_id()){
            delete_fpga_candidate(idx);
            candidate.reset(idx);
        }
    }
    // for(auto &nnode : node->get_neighbor_node()){
    //     if(!nnode->get_is_placed()){
    //         auto &nn_candidate = nnode->get_candidate();
    //         auto &fpga_candidate = m_reachable_FPGAs[fpga->get_fpga_id()][0];
    //         for(int idx = 0; idx < m_fpga.size(); idx++){
    //             if (nn_candidate[idx] == 1 && fpga_candidate[idx] == 0){
    //                 delete_fpga_candidate(idx);
    //                 nn_candidate.reset(idx);
    //             }
    //         }
    //     }
    // }
    if(fpga->is_full()){
        set_full_fpga_candidate(fpga);
    }
}

void FPGA_Partition::fixed_node_placement(){
    for(auto fixed_id : m_fixed_node) {
        FPGA_Node *node = m_DesignMgr.get_NodeMap()[std::to_string(fixed_id)];
        if(node->get_is_fixed()){
            set_node_on_fpga(node, m_fpga_map[node->get_fixed_fpga_id()]);
        }
    }
}

void FPGA_Partition::sort_node_fpga_candidate(std::vector<std::pair<int, int>> &node_fpga_candidate, FPGA_Node *node){
    for(size_t idx = 0; idx < m_fpga.size(); idx++){
        if(node->get_candidate()[idx] == 1){
            int extern_degree = get_external_degree(node, m_fpga[idx]);
            // std::cout << "m_fpga_candidate[idx].second*m_fpga.size() : " << m_fpga_candidate[idx].second << '\n';
            // std::cout << "extern_degree: " << extern_degree << '\n';
            node_fpga_candidate.push_back(std::make_pair(idx, m_fpga_candidate[idx].second + extern_degree*10));
        }
    }
    
    if(node_fpga_candidate.empty()){ //avoid candidate all 0
        for(auto &fpga : m_fpga){
            if(!fpga->is_full()){
                node_fpga_candidate.push_back(std::make_pair(fpga->get_fpga_id(), 0));
                break;
            }
        }
    }
    if(node_fpga_candidate.size() > 1){
        std::sort(node_fpga_candidate.begin(), node_fpga_candidate.end(), node_fpga_candidate_cmp);
    }
    assert(!node_fpga_candidate.empty());
}

void FPGA_Partition::neighbor_of_fixed_node_placement(){
    for(auto fixed_id : m_fixed_node) {
        FPGA_Node *fixed_node = m_DesignMgr.get_NodeMap()[std::to_string(fixed_id)];
        std::vector<FPGA_Node *> neighbor_node;
        neighbor_node.reserve(fixed_node->get_neighbor_node().size());
        for(auto &nnode : fixed_node->get_neighbor_node()){
            if(!nnode->get_is_placed()){
                neighbor_node.push_back(nnode);
            }   
        }
        //sort by bitset count from small to large.
        std::sort(neighbor_node.begin(), neighbor_node.end(), node_candidate_cmp);

        for(auto &nnode : neighbor_node){
            if(!fixed_node->get_fpga()->is_full() && 
                nnode->get_candidate()[fixed_node->get_fpga()->get_fpga_id()] == 1){
                set_node_on_fpga(nnode, fixed_node->get_fpga());
            } else {
                //set fixed node fpga candidate to 0
                nnode->get_candidate().reset(fixed_node->get_fpga()->get_fpga_id());
                delete_fpga_candidate(fixed_node->get_fpga()->get_fpga_id());

                //sort from small to large by fpga candidate
                // assert(nnode->get_candidate().count() > 0);
                std::vector<std::pair<int, int>> node_fpga_candidate;//<fpga id, fpga candidate>
                sort_node_fpga_candidate(node_fpga_candidate, nnode);
                
                //place to the fpga with least fpga candidate num
                set_node_on_fpga(nnode, m_fpga_map[node_fpga_candidate.front().first]);
            }
        }

    }
}

void FPGA_Partition::update_movable_node_fpga_candidate(FPGA_Node* node, BITSET &final_cand){
    assert(!node->get_is_placed());
    auto &origin_cand = node->get_candidate();
    for(size_t idx = 0; idx < m_fpga.size(); idx++){
        if(origin_cand[idx] == 1 && final_cand[idx] == 0){
            origin_cand.reset(idx);
            delete_fpga_candidate(idx);
        }
        if(origin_cand[idx] == 0 && final_cand[idx] == 1){
            origin_cand.set(idx);
            add_fpga_candidate(idx);
        }
    }
    node->set_candidate(final_cand);
}

int FPGA_Partition::get_external_degree(FPGA_Node* node, FPGA_fpga* fpga){
    int total_degree = 0;
    std::set<int> placed_fpga_id;
    for(auto n_id : node->get_net_id()){
        auto &net = m_DesignMgr.get_NetMap()[n_id];
        for(auto &nnode : net->get_all_nodes()){
            if(nnode->get_node_id() == node->get_node_id()) continue;
            if(nnode->get_is_placed() && (nnode->get_fpga()->get_fpga_id() != fpga->get_fpga_id())){
                total_degree ++;
            }
        }
    }
    return total_degree;
}

void FPGA_Partition::movable_node_placement(){
    //choose the least candidate cnt node from the remaining node
    std::vector<FPGA_Node *> remaining_node;
    for(auto &node : m_DesignMgr.get_fpga_nodes()){
        if(!node->get_is_placed()){
            remaining_node.push_back(node);
        }
    }
    std::sort(remaining_node.begin(), remaining_node.end(), node_candidate_cmp);

    //untile all node placed
    while(!remaining_node.empty()){
        auto &rnode = remaining_node.front();
        std::vector<FPGA_Node *> unplaced_neighbor_node;
        std::vector<FPGA_Node *> placed_neighbor_node;
        BITSET unplaced_nn_cand, placed_nn_cand, updated_cand, final_cand;
        unplaced_nn_cand.reset();
        placed_nn_cand.reset();
        updated_cand.reset();
        final_cand.reset();
        if(!rnode->get_neighbor_node().empty()){
            for(auto &nnode : rnode->get_neighbor_node()){
                if(!nnode->get_is_placed()){
                    unplaced_neighbor_node.push_back(nnode);
                } else {
                    placed_neighbor_node.push_back(nnode);
                }
            }
            
            //turn on fillter
            if(!unplaced_neighbor_node.empty()){ //purpose: reduce candidate count
                //union unplaced neighbor node
                for(auto &unnode : unplaced_neighbor_node){
                    unplaced_nn_cand |= unnode->get_candidate();
                }
                updated_cand = unplaced_nn_cand & rnode->get_candidate();
            } else {
                updated_cand = rnode->get_candidate();
            }
            //turn off fillter
            //updated_cand = rnode->get_candidate();

            //union placed neightbor node
            if (updated_cand.count() > 0) {
                for(auto &pnnode : placed_neighbor_node){
                    placed_nn_cand |= pnnode->get_candidate();
                }
                final_cand = updated_cand & placed_nn_cand;

                if(final_cand.count() == 0){
                    final_cand = updated_cand;
                }
                update_movable_node_fpga_candidate(rnode, final_cand);
            }
        }
        
        // turn on function no_cddt_node_placement
        // assert(rnode->get_candidate().count() > 0);
        // if(rnode->get_candidate().count() <= 0){
        //     remaining_node.erase(remaining_node.begin());
        //     continue;
        // }

        std::vector<std::pair<int, int>> node_fpga_candidate;//<fpga id, fpga candidate>
        sort_node_fpga_candidate(node_fpga_candidate, rnode);

        // //place to the fpga with least fpga candidate num
        assert(m_fpga_map[node_fpga_candidate.front().first] != nullptr);
        set_node_on_fpga(rnode, m_fpga_map[node_fpga_candidate.front().first]);

        remaining_node.erase(remaining_node.begin());
    }
}


void FPGA_Partition::no_cddt_node_placement(){
    for(auto &ncddt_node : m_DesignMgr.get_fpga_nodes()){
        if(!ncddt_node->get_is_placed()){
            bool is_placed = false;
            std::vector<std::pair<int, int>> fpga_cddt;
            fpga_cddt.reserve(m_fpga.size());
            for(size_t idx = 0; idx < m_fpga.size(); idx++){
                fpga_cddt.push_back(std::make_pair(idx, 0));
            }
            for(auto &nnode : ncddt_node->get_neighbor_node()){
                if(nnode->get_is_placed() && !nnode->get_fpga()->is_full()){
                    fpga_cddt[nnode->get_fpga()->get_fpga_id()].second++;
                }
            }
            std::sort(fpga_cddt.begin(), fpga_cddt.end(), no_cddt_node_placement_cmp);
            for(auto &cddt : fpga_cddt){
                if(!m_fpga_map[cddt.second]->is_full()){
                    set_node_on_fpga(ncddt_node, m_fpga_map[cddt.second]);
                    is_placed = true;
                    break;
                }
            }
            if(!is_placed){
                for(auto &fpga: m_fpga){
                    if(!fpga->is_full()){
                        set_node_on_fpga(ncddt_node, fpga);
                        break;
                    }
                }
            }
        }
    }
}

}