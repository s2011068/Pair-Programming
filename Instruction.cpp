#include<iostream>
#include"Instruction.h"

using namespace std;

void Instruction::handle_inst(int argc, char* argv[]) 
{
    ShuduGame shuduGame;
    if(strcmp(argv[1], "-c") == 0)
        if(argc == 3)
            shuduGame.init_shuduGame((long long)argv[2]);
        else
            cout<<"error: please input the map's num!"<<endl;
    else if(strcmp(argv[1], "-s") == 0)
    {
        if(argc == 3)
            shuduGame.solve_shuduGame(argv[2]);
        else
            cout<<"error: please input the file path!"<<endl;
    }
    else
        cout<<"error: inst '"<<argv[1]<<"' doesn't exist!"<<endl;

}
