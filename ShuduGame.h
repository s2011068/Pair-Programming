#include"GameMap.h"

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
