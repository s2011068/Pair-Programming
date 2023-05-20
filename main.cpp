#include"ShuduGame.h"
#include"GameMap.h"
#include"Instruction.h"


int main(int argc, char* argv[])
{
    Instruction inst;
    ShuduGame shuduGame;
    inst.handle_inst(shuduGame, argc, argv);
    return 0;
}
