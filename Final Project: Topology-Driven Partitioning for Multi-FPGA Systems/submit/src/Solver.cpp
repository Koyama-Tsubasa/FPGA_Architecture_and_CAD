#include "../include/Solver.hpp"

namespace FPGA_BASE {

void FPGA_Solver::solve(){

	std::cout << "========================" << std::endl;
	std::cout << "First Stage: Propagation" << std::endl;
	std::cout << "========================" << std::endl;
	FPGA_Propagation Propagation(DesignMgr);
	auto AfterPropagate = Propagation.start_propagate();

	std::cout << "==========================" << std::endl;
	std::cout << "Second Stage: Partitioning" << std::endl;
	std::cout << "==========================" << std::endl;

	FPGA_Partition Partition(DesignMgr, AfterPropagate.get());
	Partition.start_partition();
}

} //FPGA_BASE