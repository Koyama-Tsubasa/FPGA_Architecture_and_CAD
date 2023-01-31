# FPGA Architecture & CAD
--  How to Compile
    In this directory, enter the following command:
    $ make 
    It will generate the executable file "topart" in "FPGA-Architecture-CAD/bin".

    if you want to remove it, please enter the following command:
    $ make clean

--  How to Run
    In this directory, enter the following command:
    Usage: bin/<exe> input/<testcase file> output/<output file>
    e.g.:
    $ bin/topart input/B1.txt output/output1.txt 

    In "FPGA-Architecture-CAD/bin/", enter the following command:
    Usage: ./<exe> ../input/<testcase file> ../output/<output file>
    e.g.:
    $ ./topart ../input/B1.txt ../output/output1.txt