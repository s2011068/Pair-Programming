#include<iostream>
#include<vector>
#include<string.h>
#include<fstream>
#include <stack>

using namespace std;

const int MapSize = 9;

class GameMap {
public:
    int gameMap[MapSize][MapSize];
public:
    void generate_gameMap();
    void save_gameMap();
};

void GameMap::generate_gameMap() 
{

}

void GameMap::save_gameMap() 
{

}

class ShuduGame {
private:
    vector<GameMap> gameMaps;
    GameMap gameMap;
    bool mapRow[MapSize][MapSize]; //记录每个数字出现在哪一行
    bool mapCloumn[MapSize][MapSize]; //记录每个数字出现在哪一列
    bool mapBlock[MapSize][MapSize/3][MapSize/3]; //记录每个数字出现在哪一块（3*3）
    stack<pair<int, int>> mapSpace; //记录每个空格的位置
    int gameMapsNum = 0;
public:
    void init_sudoGame(int count);
    void solve_sudoGame(string path);
    bool test_sudoGame();
    void load_sudoGame(string path);
};

void ShuduGame::init_sudoGame(int count)
{
    //初始化数独游戏，生成若干个数独终盘
    for(int i = 0; i < count; i++)
    {
        gameMaps[i].generate_gameMap();
    }
}

void ShuduGame::load_sudoGame(string path)
{
    //从文件中读取若干个数独游戏
    gameMapsNum = 1;
}

bool ShuduGame::test_sudoGame()
{
    //以递归的方式尝试数独棋盘的可能性
    //当发现没有可以填入的数字，就采用回溯的方法，直到棋盘中所有的空格都被填满
    //我们将会找到所有的数独解法
    if(mapSpace.empty())
    {
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                cout<<gameMap.gameMap[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<"-----------------"<<endl;
        return true;
    }

    int i = (mapSpace.top()).first;
    int j = (mapSpace.top()).second;

    for(int number = 0; number < 9; number++)
    {
        if(!mapRow[number][i] && !mapCloumn[number][j] && !mapBlock[number][i/3][j/3])
        {
            mapSpace.pop();
            mapRow[number][i] = true;
            mapCloumn[number][j] = true;
            mapBlock[number][i/3][j/3] = true;
            gameMap.gameMap[i][j] = number + 1;
            test_sudoGame();
            mapSpace.push(pair<int, int>(i,j));
            mapRow[number][i] = false;
            mapCloumn[number][j] = false;
            mapBlock[number][i/3][j/3] = false;
        }
    }
    return false;
}

void ShuduGame::solve_sudoGame(string path)
{   
    for(int count = 0; count < gameMapsNum; count++)
    {
        gameMap = gameMaps[count];
        for(int i = 0; i < MapSize; i++)
        {
            for(int j = 0; j < MapSize; j++)
            {
                mapRow[i][j] = false;
                mapCloumn[i][j] = false;
            }
        }

        for(int i = 0; i < MapSize; i++)
            for(int j = 0; j < MapSize/3; j++)
                for(int k = 0;k < MapSize/3; k++)
                    mapBlock[i][j][k] = false;
        //对当前数独棋盘信息进行统计，找出每个数字所在的行和列以及空格的位置
        for(int i = 0; i < MapSize; i++)
        {
            for(int j = 0; j < MapSize; j++)
            {
                if(gameMap.gameMap[i][j])
                {
                    int num = gameMap.gameMap[i][j] - 1;
                    mapRow[num][i] = true;
                    mapCloumn[num][j] = true;
                    mapBlock[num][i/3][j/3] = true;
                }
                else
                    mapSpace.push(pair<int, int>(i,j));
            }
        }
        test_sudoGame();
    }
}

class Instruction {
public:
    void handle_inst(ShuduGame shuduGame, int argc, char* argv[]);
};

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

int main(int argc, char* argv[])
{
    Instruction inst;
    ShuduGame shuduGame;
    inst.handle_inst(shuduGame, argc, argv);
    return 0;
}
