// Copyright [2023] <Copyright albert>
#include<string.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<set>
#include<string>
#include<fstream>
#include <stack>
#include<ctime>
#include<random>
#include<cstdlib>

#define FIRSTMAX 362880

using namespace std;

const int MapSize = 9;
static int firstRow[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
// string endfilename = "endMaps.txt";
// string gamefilename = "gameMaps.txt";

class GameMap {
public:
    vector<vector<int> > gameMap;
    int shift[8] = { 3, 6, 1, 4, 7, 2, 5, 8 };
public:
    GameMap();  // 默认构造函数
    GameMap(const GameMap& existMap);  // 拷贝构造函数
    void generate_endMap(int firstRow[]);  // 生成数独终局
    void add_endGrids(int n);
    void generate_gameMap(int minHoles, int maxHoles, int mode, bool uFlag);
    void digHoles(GameMap& endMap, int holes, bool uFlag);
    bool isNumberInRow(const GameMap& shudu, int row, int number);  // 检查数字是否在行中重复
    bool isNumberInColumn(const GameMap& shudu, int col, int number);  // 检查数字是否在列中重复
    bool isNumberInBox(const GameMap& shudu, int startRow, int startCol, int number);  // 检查数字是否在九宫格中重复
    bool isNumberValid(const GameMap& shudu, int row, int col, int number);  // 检查数字在当前位置是否合法
    bool isShuduSolved(const GameMap& shudu);  // 检查数独是否已经解决完毕
    bool solveShudu(GameMap& shudu, int& solutionCount);
};

// 自定义比较函数
struct SudoCompare {
    bool operator()(const GameMap& obj1, const GameMap& obj2) const {
        return obj1.gameMap < obj2.gameMap;
    }
};

set<GameMap, SudoCompare> endGrids, gameGrids;

// 生成指定范围内的随机数
int getRandomNumber(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

GameMap::GameMap() {
    gameMap.resize(MapSize, vector<int>(MapSize));
    for (int row = 0; row < MapSize; row++) {
        for (int col = 0; col < MapSize; col++) {
            gameMap[row][col] = 0;
        }
    }
}

GameMap::GameMap(const GameMap& existedGrid) {
    gameMap.resize(MapSize, vector<int>(MapSize));
    for (int row = 0; row < MapSize; row++) {
        for (int col = 0; col < MapSize; col++) {
            this->gameMap[row][col] = existedGrid.gameMap[row][col];
        }
    }
}

void GameMap::generate_endMap(int firstRow[]) {
    // 将第一行的排列应用到数独终局中
    for (int i = 0; i < MapSize; i++) {
        gameMap[0][i] = firstRow[i];
    }
    // 根据第一行的排列生成数独终局
    for (int row = 1; row < MapSize; row++) {
        for (int col = 0; col < MapSize; col++) {
            gameMap[row][col] = gameMap[0][(col + shift[row - 1]) % 9];
        }
    }
    return;
}

void GameMap::add_endGrids(int n) {
    while (endGrids.size() < n && endGrids.size() < FIRSTMAX) {
        this->generate_endMap(firstRow);
        endGrids.insert(*this);
        // cout << "终局插入了" << endGrids.size()<<endl;
        next_permutation(firstRow, firstRow + 9);
    }
    while (endGrids.size() < n) {
        for (const auto& existingGrid : endGrids) {
            GameMap newMap(existingGrid);
            swap(newMap.gameMap[1], newMap.gameMap[2]);
            endGrids.insert(newMap);
            if (endGrids.size() == n) break;
            GameMap new2Map(existingGrid);
            swap(new2Map.gameMap[4], new2Map.gameMap[5]);
            endGrids.insert(new2Map);
            if (endGrids.size() == n) break;
            GameMap new3Map(existingGrid);
            swap(new3Map.gameMap[7], new3Map.gameMap[8]);
            endGrids.insert(new3Map);
            if (endGrids.size() == n) break;
        }
    }
}

void GameMap::generate_gameMap(int minHoles, int maxHoles, int mode, bool uFlag = false) {
    for (GameMap grid : endGrids) {
        if (mode == 0 && minHoles == 0 && maxHoles == 0) {
            minHoles = 25;
            maxHoles = 35;
        }
        if (mode == 1) {
            minHoles = 20;
            maxHoles = 30;
        }
        if (mode == 2) {
            minHoles = 31;
            maxHoles = 44;
        }
        if (mode == 3) {
            minHoles = 45;
            maxHoles = 55;
        }
        cout << "Game Mode:" << mode << endl;
        cout << "Scope of holes:" << minHoles << "~" << maxHoles << endl;
        cout << "Is there a unique solution:" << uFlag << endl;
        int holes = getRandomNumber(minHoles, maxHoles);
        digHoles(grid, holes, uFlag);
        gameGrids.insert(grid);
    }
}

void GameMap::digHoles(GameMap& grid, int holes, bool uFlag = false) {
    cout << "Holes:" << holes << endl;
    int count = 0;
    while (count < holes) {
        int row = getRandomNumber(0, MapSize - 1);
        int col = getRandomNumber(0, MapSize - 1);
        if (uFlag == false) {  // 不保证唯一解
            if (grid.gameMap[row][col] != 0) {
                grid.gameMap[row][col] = 0;
            }
        }
        else {  // 不保证唯一解
            if (grid.gameMap[row][col] != 0) {
                // cout << "进入挖空了" << endl;
                int temp = grid.gameMap[row][col];
                grid.gameMap[row][col] = 0;

                GameMap tempShudu = grid;

                // 检查挖空后是否有唯一解
                int solutionCount = 0;
                solveShudu(tempShudu, solutionCount);
                if (solutionCount != 1) {
                    grid.gameMap[row][col] = temp;  // 还原挖空前的数字
                }
            }
        }
        count++;
    }
}


bool GameMap::isNumberInRow(const GameMap& shudu, int row, int number) {
    for (int col = 0; col < MapSize; col++) {
        if (shudu.gameMap[row][col] == number) {
            return true;
        }
    }
    return false;
}

bool GameMap::isNumberInColumn(const GameMap& shudu, int col, int number) {
    for (int row = 0; row < MapSize; row++) {
        if (shudu.gameMap[row][col] == number) {
            return true;
        }
    }
    return false;
}

bool GameMap::isNumberInBox(const GameMap& shudu, int startRow, int startCol, int number) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (shudu.gameMap[row + startRow][col + startCol] == number) {
                return true;
            }
        }
    }
    return false;
}

bool GameMap::isNumberValid(const GameMap& shudu, int row, int col, int number) {
    return !isNumberInRow(shudu, row, number) &&
        !isNumberInColumn(shudu, col, number) &&
        !isNumberInBox(shudu, row - row % 3, col - col % 3, number);
}

bool GameMap::isShuduSolved(const GameMap& shudu) {
    for (int row = 0; row < MapSize; row++) {
        for (int col = 0; col < MapSize; col++) {
            if (shudu.gameMap[row][col] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool GameMap::solveShudu(GameMap& shudu, int& solutionCount) {
    // cout << "进入求解" << endl;
    for (int row = 0; row < MapSize; row++) {
        for (int col = 0; col < MapSize; col++) {
            if (shudu.gameMap[row][col] == 0) {
                for (int number = 1; number <= MapSize; number++) {
                    if (isNumberValid(shudu, row, col, number)) {
                        shudu.gameMap[row][col] = number;
                        if (solveShudu(shudu, solutionCount)) {
                            if (solutionCount > 1) {
                                return true;  // 有多个解，停止求解
                            }
                        }
                        shudu.gameMap[row][col] = 0;  // 回溯
                    }
                }
                return false;
            }
        }
    }

    if (isShuduSolved(shudu)) {
        solutionCount++;
        return true;
    }
    return false;
}

static void saveToFile(const set<GameMap, SudoCompare>& uniqueObjects, const string& filename) {
    ofstream file(filename);
    int mycount = 1;
    if (file.is_open()) {
        for (const auto& obj : uniqueObjects) {
            file << "Map" << mycount << "---------------------------" << endl;;
            for (const auto& row : obj.gameMap) {
                for (int val : row) {
                    string cval;
                    cval = to_string(val);
                    if (cval == "0") 
                        cval = "$";
                    file << cval << " ";
                }
                file << endl;
            }
            mycount++;
        }
        file.close();
        cout << "Map data saved to file: " << filename << endl;
    }
    else {
        cerr << "Failed to open file: " << filename << endl;
    }
}

class ShuduGame {
private:
    vector<GameMap> gameMaps;
    GameMap gameMap;
    bool mapRow[MapSize][MapSize];  // 记录每个数字出现在哪一行
    bool mapCloumn[MapSize][MapSize];  // 记录每个数字出现在哪一列
    bool mapBlock[MapSize][MapSize / 3][MapSize / 3];  // 记录每个数字出现在哪一块（3*3）
    stack<pair<int, int>> mapSpace;  // 记录每个空格的位置
    int gameMapsNum = 0;
    int solutionIndex = 0;
    int gameIndex = 0;
    int solutionNum = 0;
public:
    ShuduGame();
    void solve_shuduGame(string path, int solutionNum);
    bool test_shuduGame();
    void load_shuduGame(string path);
    void save_shuduGame();
};

ShuduGame::ShuduGame() {
    for (int i = 0; i < MapSize; i++) {
        for (int j = 0; j < MapSize; j++) {
            mapRow[i][j] = false;
            mapCloumn[i][j] = false;
        }
    }

    for (int i = 0; i < MapSize; i++)
        for (int j = 0; j < MapSize / 3; j++)
            for (int k = 0; k < MapSize / 3; k++)
                mapBlock[i][j][k] = false;
}

void ShuduGame::load_shuduGame(string path) {
    // 从文件中读取若干个数独游戏
    // cout<<"开始读取！"<<path<<endl;
    ifstream file(path);
    string line;
    int index = 0;
    GameMap gameMap;
    while (getline(file, line)) {
        if (index % 10) {
            for (int i = 0; i < MapSize; i++) {
                if (line[i * 2] == '$')
                    gameMap.gameMap[index % 10 - 1][i] = 0;
                else
                    gameMap.gameMap[index % 10 - 1][i] = (line[i * 2] - '0');
            }
        }
        if ((index + 1) % 10 == 0) {
            this->gameMaps.push_back(gameMap);
        }
        index++;
    }
    gameMapsNum = index / 10;
    // cout<<"读取完成！"<<endl;
}

void ShuduGame::save_shuduGame() {
    // 从文件中读取若干个数独游戏
    if (gameIndex == 1) {
        ofstream file("shudu.txt", ios::trunc);
        file.close();
    }
    ofstream file("shudu.txt", ios::app);
    if (solutionIndex == 1) {
        file << "游戏" << gameIndex;
        file << "-----------------------" << endl;
    }
    // else
    //    file.seekp(0,ios::end);
    if (solutionIndex <= solutionNum || !solutionNum) {
        file << "解法" << solutionIndex << endl;
        for (int i = 0; i < MapSize; i++) {
            for (int j = 0; j < MapSize; j++)
                file << gameMap.gameMap[i][j] << " ";
            file << endl;
        }
    }
    // cout<<"写入完成！"<<endl;
}

bool ShuduGame::test_shuduGame() {
    // 以递归的方式尝试数独棋盘的可能性
    // 当发现没有可以填入的数字,就采用回溯的方法,直到棋盘中所有的空格都被填满
    // 我们将会找到所有的数独解法
    if (mapSpace.empty()) {
        solutionIndex++;
        save_shuduGame();
        return true;
    }

    int i = (mapSpace.top()).first;
    int j = (mapSpace.top()).second;

    for (int number = 0; number < 9; number++) {
        if (!mapRow[number][i] && !mapCloumn[number][j] && !mapBlock[number][i / 3][j / 3]) {
            mapSpace.pop();
            mapRow[number][i] = true;
            mapCloumn[number][j] = true;
            mapBlock[number][i / 3][j / 3] = true;
            gameMap.gameMap[i][j] = number + 1;
            test_shuduGame();
            mapSpace.push(pair<int, int>(i, j));
            mapRow[number][i] = false;
            mapCloumn[number][j] = false;
            mapBlock[number][i / 3][j / 3] = false;
        }
    }
    return false;
}

void ShuduGame::solve_shuduGame(string path, int sNum) {
    load_shuduGame(path);
    solutionNum = sNum;
    for (int count = 0; count < gameMapsNum; count++) {
        gameMap = gameMaps[count];
        solutionIndex = 0;
        gameIndex = count + 1;
        /*for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                cout<<gameMap.gameMap[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<"-----------------"<<endl;*/
        for (int i = 0; i < MapSize; i++) {
            for (int j = 0; j < MapSize; j++) {
                mapRow[i][j] = false;
                mapCloumn[i][j] = false;
            }
        }

        for (int i = 0; i < MapSize; i++)
            for (int j = 0; j < MapSize / 3; j++)
                for (int k = 0; k < MapSize / 3; k++)
                    mapBlock[i][j][k] = false;

        mapSpace = stack<pair<int, int>>();
        // 对当前数独棋盘信息进行统计,找出每个数字所在的行和列以及空格的位置
        for (int i = 0; i < MapSize; i++) {
            for (int j = 0; j < MapSize; j++) {
                if (gameMap.gameMap[i][j]) {
                    int num = gameMap.gameMap[i][j] - 1;
                    mapRow[num][i] = true;
                    mapCloumn[num][j] = true;
                    mapBlock[num][i / 3][j / 3] = true;
                }
                else {
                    mapSpace.push(pair<int, int>(i, j));
                }
            }
        }
        test_shuduGame();
    }
}

class Instruction {
private:
    int c = 0;  // 终局个数
    int n = 0;  // 游戏数量
    int m = 0;  // 游戏难度
    int rMin = 0;  // 挖空数量最小值
    int rMax = 0;  // 挖空数量最大值
    bool uFlag = false;  // 解唯一标志
public:
    void handle_inst(int argc, char* argv[]);
    bool isInRange(int value, int minRange, int maxRange);
};

bool Instruction::isInRange(int value, int minRange, int maxRange) {
    return (value >= minRange && value <= maxRange);
}


void Instruction::handle_inst(int argc, char* argv[]) {
    ShuduGame shuduGame;
    GameMap grid;
    if (strcmp(argv[1], "-s") == 0) {
        if (argc == 3)
            shuduGame.solve_shuduGame(argv[2], 0);
        else if (argc == 5 && strcmp(argv[3], "-N") == 0)
            shuduGame.solve_shuduGame(argv[2], argv[4][0] - '0');
        else
            cout << "error: please input the file path!" << endl;
    }
    else if (strcmp(argv[1], "-c") == 0) {
        // cout << "cvalue" << cValue << endl;
        if (argc == 3) {
            c = stoi(argv[2]);
            if (isInRange(c, 1, 1000000)) {
                grid.add_endGrids(c);  // 生成数独终局
                saveToFile(endGrids, "endMaps.txt");
                return;
            }
            else {
                cout << "The quantity exceeds the range. Please provide a value between 1-1000000" << endl;
                return;
            }
        }
        else {
            cout << "Error:The parameter is missing,please enter :shudu.exe - c <count>" << endl;
            return;
        }
    }
    else {
        for (int i = 1; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-n") {
                i++;
                if (i >= argc) {
                    cout << "Error:The parameter is missing,please enter :shudu.exe - n <count>" << endl;
                    return;
                }
                n = stoi(argv[i]);
                if (!isInRange(n, 1, 10000)) {
                    cout << "Error: Game quantity range should be between 1 and 10000" << endl;
                    return;
                }
            }
            else if (arg == "-m") {
                i++;
                if (i >= argc) {
                    cout << "Error:The parameter is missing,please enter :shudu.exe - n <count> -m <mode>" << endl;
                    return;
                }
                m = stoi(argv[i]);
                if (!isInRange(m, 1, 3)) {
                    cout << "Error: please choose 1, 2, or 3" << endl;
                    return;
                }
            }
            else if (arg == "-r") {
                i++;
                if (i >= argc) {
                    cout << "Error:please input shudu.exe - n<count>- r<minHoles~maxHoles>" << endl;
                    return;
                }
                string range = argv[i];
                size_t rangePos = range.find("~");
                if (rangePos == string::npos) {
                    cout << "Error:please input shudu.exe - n<count>- r<minHoles~maxHoles>" << endl;
                    return;
                }
                string rMinValue = range.substr(0, rangePos);
                string rMaxValue = range.substr(rangePos + 1);
                rMin = stoi(rMinValue);
                rMax = stoi(rMaxValue);
                if (!isInRange(rMin, 20, 55) || !isInRange(rMax, 20, 55) || rMin > rMax) {
                    cout << "Error:please provide a valid range between 20 and 55" << endl;
                    return;
                }
            }
            else if (arg == "-u") {
                uFlag = true;
            }
            else {
                cout << "error: inst '" << argv[1] << "' doesn't exist!" << endl;
                return;
            }
        }
        if (n == 0) {
            cout << "error: Please input n" << endl;
            return;
        }
        grid.add_endGrids(n);  // 生成数独终局
        grid.generate_gameMap(rMin, rMax, m, uFlag);
        saveToFile(gameGrids, "gameMaps.txt");
        return;
    }
}

int main(int argc, char* argv[]) {
    Instruction inst;
    inst.handle_inst(argc, argv);
    return 0;
}
