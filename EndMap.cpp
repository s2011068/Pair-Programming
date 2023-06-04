#include<iostream>
#include<vector>
#include<string.h>
#include<string>
#include<fstream>
#include<algorithm>
#include <vector>
#include <array>
#include <set>
#include<algorithm>
#include <ctime>
#include <random>

#define FIRSTMAX 362880
using namespace std;

const int MapSize = 9;
static int firstRow[9] = { 1,2,3,4,5,6,7,8,9 };
string filename = "endMaps1.txt";

class EndMap {
public:
    vector<vector<int> > endMap;
    int shift[8] = { 3, 6, 1, 4, 7, 2, 5, 8 };
public:
    EndMap();//默认构造函数
    EndMap(const EndMap& existMap);//拷贝构造函数
    void generate_endMap(int firstRow[]);// 生成数独终局
    void add_endGrids(int n);
    void generate_gameMap(int minHoles, int maxHoles, bool uFlag);
    void digHoles(EndMap& endMap, int holes, bool uFlag);
    bool isNumberInRow(const EndMap& shudu, int row, int number);// 检查数字是否在行中重复
    bool isNumberInColumn(const EndMap& shudu, int col, int number);// 检查数字是否在列中重复
    bool isNumberInBox(const EndMap& shudu, int startRow, int startCol, int number);// 检查数字是否在九宫格中重复
    bool isNumberValid(const EndMap& shudu, int row, int col, int number);// 检查数字在当前位置是否合法
    bool isShuduSolved(const EndMap& shudu);// 检查数独是否已经解决完毕
    bool solveShudu(EndMap& shudu, int& solutionCount);
};

// 自定义比较函数
struct SudoCompare {
    bool operator()(const EndMap& obj1, const EndMap& obj2) const {
        return obj1.endMap < obj2.endMap;
    }
};

set<EndMap, SudoCompare> endGrids, gameGrids;

// 生成指定范围内的随机数
int getRandomNumber(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

EndMap::EndMap() {
    endMap.resize(MapSize, vector<int>(MapSize));
    for (int row = 0; row < MapSize; row++) {
        for (int col = 0; col < MapSize; col++) {
            endMap[row][col] = 0;
        }
    }
}

EndMap::EndMap(const EndMap& existedGrid) {
    endMap.resize(MapSize, vector<int>(MapSize));
    for (int row = 0; row < MapSize; row++) {
        for (int col = 0; col < MapSize; col++) {
            this->endMap[row][col] = existedGrid.endMap[row][col];
        }
    }
}

void EndMap::generate_endMap(int firstRow[]) {
    // 将第一行的排列应用到数独终局中
    for (int i = 0; i < MapSize; i++) {
        endMap[0][i] = firstRow[i];
    }
    // 根据第一行的排列生成数独终局
    for (int row = 1; row < MapSize; row++) {
        for (int col = 0; col < MapSize; col++) {
            endMap[row][col] = endMap[0][(col + shift[row - 1]) % 9];
        }
    }
    return;
}

void EndMap::add_endGrids(int n) {
    //int num = 0;
    while (endGrids.size() < n && endGrids.size() < FIRSTMAX) {
        this->generate_endMap(firstRow);
        endGrids.insert(*this);
        next_permutation(firstRow, firstRow + 9);
        //num++;
        //if (endGrids.size() % 50000 == 0) {
        //    endtime = clock();
        //    cout << "num" << num;
        //    cout << "endGrids.size()" << endGrids.size() << "runtime:   " << double(endtime - begintime) / CLOCKS_PER_SEC << endl;
        //}
    }
    bool change = 1;
    while (endGrids.size() < n) {
        for (const auto& existingGrid : endGrids) {
            EndMap newMap(existingGrid);
            swap(newMap.endMap[1], newMap.endMap[2]);
            endGrids.insert(newMap);
            //num++;
            if (endGrids.size() == n) break;
            EndMap new2Map(existingGrid);
            swap(new2Map.endMap[4], new2Map.endMap[5]);
            endGrids.insert(new2Map);
            //num++;
            if (endGrids.size() == n) break;
            EndMap new3Map(existingGrid);
            swap(new3Map.endMap[7], new3Map.endMap[8]);
            endGrids.insert(new3Map);
            //num++;
            if (endGrids.size() == n) break;
        }
    }
}

void EndMap::generate_gameMap(int minHoles, int maxHoles, bool uFlag = false) {
    for (EndMap grid : endGrids) {
        int holes = getRandomNumber(minHoles, maxHoles);
        digHoles(grid, holes,uFlag);  
        gameGrids.insert(grid);
    }
}

void EndMap::digHoles(EndMap& grid, int holes,bool uFlag=false) {
    cout << "挖空数" << holes << endl;
    int count = 0;
    while (count < holes) {
        int row = getRandomNumber(0, MapSize - 1);
        int col = getRandomNumber(0, MapSize - 1);
        if (uFlag == false) {//不保证唯一解
            if (grid.endMap[row][col] != 0) {
                grid.endMap[row][col] = 0;
            }
        }
        else {//不保证唯一解
            if (grid.endMap[row][col] != 0) {
                //cout << "进入挖空了" << endl;
                int temp = grid.endMap[row][col];
                grid.endMap[row][col] = 0;

                EndMap tempShudu = grid;

                // 检查挖空后是否有唯一解
                int solutionCount = 0;
                solveShudu(tempShudu, solutionCount);
                if (solutionCount != 1) {
                    grid.endMap[row][col] = temp; // 还原挖空前的数字
                }
            }
        }
        count++;    
    }
}


bool EndMap::isNumberInRow(const EndMap& shudu, int row, int number) {
    for (int col = 0; col < MapSize; col++) {
        if (shudu.endMap[row][col] == number) {
            return true;
        }
    }
    return false;
}

bool EndMap::isNumberInColumn(const EndMap& shudu, int col, int number) {
    for (int row = 0; row < MapSize; row++) {
        if (shudu.endMap[row][col] == number) {
            return true;
        }
    }
    return false;
}

bool EndMap::isNumberInBox(const EndMap& shudu, int startRow, int startCol, int number) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (shudu.endMap[row + startRow][col + startCol] == number) {
                return true;
            }
        }
    }
    return false;
}

bool EndMap::isNumberValid(const EndMap& shudu, int row, int col, int number) {
    return !isNumberInRow(shudu, row, number) &&
        !isNumberInColumn(shudu, col, number) &&
        !isNumberInBox(shudu, row - row % 3, col - col % 3, number);
}

bool EndMap::isShuduSolved(const EndMap& shudu) {
    for (int row = 0; row < MapSize; row++) {
        for (int col = 0; col < MapSize; col++) {
            if (shudu.endMap[row][col] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool EndMap::solveShudu(EndMap& shudu, int& solutionCount) {
    //cout << "进入求解" << endl;
    for (int row = 0; row < MapSize; row++) {
        for (int col = 0; col < MapSize; col++) {
            if (shudu.endMap[row][col] == 0) {
                for (int number = 1; number <= MapSize; number++) {
                    if (isNumberValid(shudu, row, col, number)) {
                        shudu.endMap[row][col] = number;
                        if (solveShudu(shudu, solutionCount)) {
                            if (solutionCount > 1) {
                                return true; // 有多个解，停止求解
                            }
                        }
                        shudu.endMap[row][col] = 0; // 回溯
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

static void saveToFile(const set<EndMap, SudoCompare>& uniqueObjects, const string& filename) {
    ofstream file(filename, ios::app);
    int mycount = 1;
    if (file.is_open()) {
        for (const auto& obj : uniqueObjects) {
            file << "Map" << mycount << "---------------------------" << endl;;
            for (const auto& row : obj.endMap) {
                for (int val : row) {
                    string cval;
                    cval = to_string(val);
                    if (cval == "0") {
                        cval = "&";
                    }
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


int main() {
    int n; // 要生成的数独终局数量
    cout << "请输入要生成的数独游戏数量和难度";
    cin >> n;
    int mode;
    cin >> mode;
    if (n < 1 || n > 1000000) {
        cout << "n的范围不合法" << endl;
        return 0;
    }

    EndMap grid;
    grid.add_endGrids(n);

     //存储剩余的map到文件
    saveToFile(endGrids, "outputnew3.txt");
    if (mode == 1) {
        cout << "难度:简单" << endl;
        grid.generate_gameMap(20, 30);
    }
    if (mode == 2) {
        cout << "难度:中等" << endl;
        grid.generate_gameMap(31, 44);
    }
    if (mode == 3) {
        cout << "难度:较高" << endl;
        grid.generate_gameMap(45, 55);
    }

    //grid.generate_gameMap(16, 16,true);
    saveToFile(gameGrids, filename);

    return 0;
}

