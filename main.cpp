#include<iostream>
#include"Instruction.h"

using namespace std;

int main(int argc, char* argv[])
{
    Instruction inst;
    inst.handle_inst(argc, argv);
    return 0;
}
