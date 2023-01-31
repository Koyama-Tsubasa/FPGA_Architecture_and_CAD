#pragma once
#include <iostream>
#include <bitset>
#include "Data.hpp"
#include "Input.hpp"
#include "Propagation.hpp"
#include "DesignManager.hpp"
#include "Partition.hpp"

namespace FPGA_BASE {

	class FPGA_Solver {
		public:
			FPGA_Solver(Input *InputPtr) :
				m_InputPtr(InputPtr),
				DesignMgr(InputPtr)
				{}
			const FPGA_DesignMgr &getDesignMgr() const { return DesignMgr; }
			void solve();
		private:
			Input *m_InputPtr;
			FPGA_DesignMgr DesignMgr;

	};

} //FPGA_BASE