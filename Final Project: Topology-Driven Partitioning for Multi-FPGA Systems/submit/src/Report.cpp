#include "Report.h"

namespace FPGA_BASE 
{

void FPGA_Report::report()
{   
    m_InputPtr->get_fpga_parameter().report();
    std::cout << "==========\n";
    std::vector<FPGA_Channel> fpga_channels = m_InputPtr->get_fpga_channels();
    for(size_t i = 0; i < m_InputPtr->get_fpga_parameter().get_fpga_channel_nums(); i++){
        fpga_channels[i].report();
    }
    std::cout << "==========\n";
    std::vector<FPGA_Net*> fpga_nets = m_InputPtr->get_fpga_nets();
    for(auto &net_iter : fpga_nets){
        net_iter->report();
    }
}

}
