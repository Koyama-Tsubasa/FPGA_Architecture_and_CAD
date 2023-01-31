#include "../include/DesignManager.hpp"

namespace FPGA_BASE {

FPGA_DesignMgr::FPGA_DesignMgr(Input *InputPtr){
    m_InputPtr = InputPtr;
    m_fpga_parameter = InputPtr->get_fpga_parameter();
    m_fpga_channels = InputPtr->get_fpga_channels();
    m_fpga_nets = InputPtr->get_fpga_nets();
    m_fpga_nodes = InputPtr->get_fpga_nodes();
    m_NodeMap = InputPtr->get_NodeMap();
    m_NetMap = InputPtr->get_NetMap();
}

void FPGA_DesignMgr::to_ostream(std::ostream &out) const{
    for(auto &node : m_fpga_nodes){
        if(node->get_is_placed()){
            out << node->get_node_id() << " " << node->get_fpga()->get_fpga_id() << '\n';
        } else {
            out << node->get_node_id() << " " << "Not placed error\n";
        }
    }
}

} //FPGA_BASE