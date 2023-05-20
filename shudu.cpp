#include<iostream>
#include<vector>
#include<string.h>
#include<fstream>

using namespace std;

const int MapSize = 9;

class GameMap {
private:
    int gameMap[MapSize][MapSize];
public:
    void generate_gameMap();
    void save_gameMap();
    void load_gameMap(string path);
};

void GameMap::generate_gameMap() 
{

}

void GameMap::save_gameMap() 
{

}

void GameMap::load_gameMap(string path) 
{

}

class ShuduGame {
private:
    GameMap gameMap;
public:
    void init_sudoGame();
    void solve_sudoGame(string path);
};

void ShuduGame::init_sudoGame()
{

}

void ShuduGame::solve_sudoGame(string path)
{
    gameMap.load_gameMap(path);
}

class Instruction {
public:
    void handle_inst(ShuduGame shuduGame, int argc, char* argv[]);
};

void Instruction::handle_inst(ShuduGame shuduGame, int argc, char* argv[]) 
{
    if(strcmp(argv[1], "-c") == 0)
        shuduGame.init_sudoGame();
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

int main(int argc, char* argv[])
{
    cout<<argc<<endl;
    Instruction inst;
    ShuduGame shuduGame;
    inst.handle_inst(shuduGame, argc, argv);
    return 0;
}
