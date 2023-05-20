#include<iostream>
#include<vector>
#include<string.h>
#include<fstream>
#include <stack>
#include"ShuduGame.h"
#include"GameMap.h"
#include"Instruction.h"


void Instruction::handle_inst(ShuduGame shuduGame, int argc, char* argv[]) 
{
    if(strcmp(argv[1], "-c") == 0)
        if(argc == 3)
            shuduGame.init_sudoGame(int(argv[2]));
        else
            cout<<"error: please input the map's num!"<<endl;
    else if(strcmp(argv[1], "-s") == 0)
    {
        if(argc == 3)
            shuduGame.solve_sudoGame(argv[2]);
        else
            cout<<"error: please input the file path!"<<endl;
    }
    else
        cout<<"error: inst '"<<argv[1]<<"' doesn't exist!"<<endl;

}
