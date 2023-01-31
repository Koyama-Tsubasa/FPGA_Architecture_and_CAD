#pragma once
#include <fstream>
#include <assert.h>
#include <stdarg.h>
#include <string>
#include <cstdio>
#include <iostream>
#include "Input.hpp"

namespace FPGA_BASE
{

class FPGA_Report
{
    public:
        FPGA_Report(Input *InputPtr) :
            m_InputPtr(InputPtr)
            {}
        void report();
    private:
        Input *m_InputPtr;
};

}
