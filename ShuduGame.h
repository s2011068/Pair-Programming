#include"GameMap.h"
#include<iostream>
#include<vector>
#include<string.h>
#include <stack>

using namespace std;

class ShuduGame {
private:
    vector<GameMap> gameMaps;
    GameMap gameMap;
    bool mapRow[MapSize][MapSize]; //记录每个数字出现在哪一行
    bool mapCloumn[MapSize][MapSize]; //记录每个数字出现在哪一列
    bool mapBlock[MapSize][MapSize/3][MapSize/3]; //记录每个数字出现在哪一块（3*3）
    stack<pair<int, int>> mapSpace; //记录每个空格的位置
    int gameMapsNum = 0;
    int solutionIndex = 0;
    int gameIndex = 0;
public:
    void init_shuduGame(int count);
    void solve_shuduGame(string path);
    bool test_shuduGame();
    void load_shuduGame(string path);
    void save_shuduGame();
};
