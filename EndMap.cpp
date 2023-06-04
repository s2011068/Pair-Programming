#include<iostream>
#include<vector>
#include<string.h>
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
static char firstRow[9] = { '1','2','3','4','5','6','7','8','9' };
//string filename = "endMaps1.txt";
time_t begintime;
time_t endtime;

class EndMap {
public:
    vector<vector<char> > endMap;
    int shift[8] = { 3, 6, 1, 4, 7, 2, 5, 8 };
public:
    EndMap();//默认构造函数
    EndMap(const EndMap& existMap);//拷贝构造函数
    void generate_endMap(char firstRow[]);// 生成数独终局
    void add_endGrids(int n);
    void generate_gameMap(int minHoles, int maxHoles);
    void digHoles(EndMap& endMap, int holes);
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
    endMap.resize(MapSize, vector<char>(MapSize));
    for (int row = 0; row < MapSize; row++) {
        for (int col = 0; col < MapSize; col++) {
            endMap[row][col] = 0;
        }
    }
}

EndMap::EndMap(const EndMap& existedGrid) {
    endMap.resize(MapSize, vector<char>(MapSize));
    for (int row = 0; row < MapSize; row++) {
        for (int col = 0; col < MapSize; col++) {
            this->endMap[row][col] = existedGrid.endMap[row][col];
        }
    }
}

void EndMap::generate_endMap(char firstRow[]) {
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
    int num = 0;
    while (endGrids.size() < n && endGrids.size() < FIRSTMAX) {
        this->generate_endMap(firstRow);
        endGrids.insert(*this);
        next_permutation(firstRow, firstRow + 9);
        num++;
        if (endGrids.size() % 50000 == 0) {
            endtime = clock();
            cout << "num" << num;
            cout << "endGrids.size()" << endGrids.size() << "runtime:   " << double(endtime - begintime) / CLOCKS_PER_SEC << endl;
        }
    }
    //cout << "endGrids.size()" << endGrids.size() << "-------------------------------------------------------------- - " << endl;
    //cout << "num:" << num << "-------------------------------------------------------------- - " << endl;
    bool change = 1;
    while (endGrids.size() < n) {
        for (const auto& existingGrid : endGrids) {
            EndMap newMap(existingGrid);
            swap(newMap.endMap[1], newMap.endMap[2]);
            endGrids.insert(newMap);
            num++;
            if (endGrids.size() == n) break;
            EndMap new2Map(existingGrid);
            swap(new2Map.endMap[4], new2Map.endMap[5]);
            endGrids.insert(new2Map);
            num++;
            if (endGrids.size() == n) break;
            EndMap new3Map(existingGrid);
            swap(new3Map.endMap[7], new3Map.endMap[8]);
            endGrids.insert(new3Map);
            num++;
            if (endGrids.size() == n) break;
            //if (endGrids.size() % 50000 == 0) {
            //    endtime = clock();
            //    cout << "num:"<<num<<"  endGrids.size():" << endGrids.size() << "   runtime:" << double(endtime - begintime) / CLOCKS_PER_SEC << endl;
            //}            
            //if (change && endGrids.size() != num) {
            //    cout << "\n\n" << num << "开始变了\n\n" << endl;
            //    change = 0;
            //}
            //if (endGrids.size() >= 900000) {
            //    if (endGrids.size() % 10000 == 0)
            //    {
            //        endtime = clock();
            //        cout << "n"<<n<<"num:" << num << "  endGrids.size():" << endGrids.size() << "   runtime:" << double(endtime - begintime) / CLOCKS_PER_SEC << endl;
            //    }
            //}

        }
    }
    //cout << "endGrids.size():" << endGrids.size() << "-------------------------------------------------------------- - " << endl;
    //cout << "num:" << num << "-------------------------------------------------------------- - " << endl;
}

void EndMap::generate_gameMap(int minHoles, int maxHoles) {
    for (EndMap grid : endGrids) {
        int holes = getRandomNumber(minHoles, maxHoles);
        digHoles(grid, holes);  
        gameGrids.insert(grid);
    }

}

void EndMap::digHoles(EndMap& grid, int holes) {
    //cout << "挖空" << holes << endl;
    int count = 0;
    while (count < holes) {
        int row = getRandomNumber(0, MapSize - 1);
        int col = getRandomNumber(0, MapSize - 1);
        if (grid.endMap[row][col] != '&') {
            grid.endMap[row][col] = '&';
            //cout << "替换了" << endl;
        }
        count++;
        cout << count << endl;

    }
}

static void saveToFile(const set<EndMap, SudoCompare>& uniqueObjects, const string& filename) {
    ofstream file(filename, ios::app);
    int mycount = 1;
    if (file.is_open()) {
        for (const auto& obj : uniqueObjects) {
            file << "endMap" << mycount << "---------------------------" << endl;;
            for (const auto& row : obj.endMap) {
                for (char val : row) {
                    file << val << " ";
                }
                file << endl;
            }
            if (mycount % 50000 == 0) {
                endtime = clock();
                cout << "runtime:   " << double(endtime - begintime) / CLOCKS_PER_SEC << "   ,";
                cout << mycount << "个数独保存了" << endl;
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
    cout << "请输入要生成的数独终局数量：";
    cin >> n;

    EndMap grid;
    begintime = clock();
    grid.add_endGrids(n);

    // 存储剩余的map到文件
    //saveToFile(endGrids, "outputnew3.txt");

    grid.generate_gameMap(20, 25);
    saveToFile(gameGrids, "outputnew3.txt");

    return 0;
}

