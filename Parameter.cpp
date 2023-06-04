#include<iostream>
#include<string>
#include<cstdlib>
#include<string.h>
using namespace std;
class Instruction {
private:
    int c = 0;//终局个数
    int n= 0;//游戏数量
    int m = 0;//游戏难度
    int rMin = 0;//挖空数量最小值
    int rMax = 0;//挖空数量最大值
    bool uFlag = false;  // 解唯一标志
public:
    void handle_inst(int argc, char* argv[]);
    bool isVaildParameter(const string& parameter);//检查参数是否有效
    string getParameterValue(int argc, char* argv[], const string& parameter);//获取参数值
    bool isInRange(int value, int minRange, int maxRange);
};
bool Instruction::isVaildParameter(const string& parameter) {
    if (parameter.empty() || parameter[0] != '-')
        return false;
    return true;
}

string Instruction::getParameterValue(int argc, char* argv[], const string& parameter) {
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == parameter)
        {
            //cout << "argv" << i << " is "<<argv[i]<<endl;
            //cout << "argv" << i + 1 << "is" << argv[i + 1] << endl;
            if (i + 1 < argc && isVaildParameter(argv[i]))
                return argv[i + 1];
            else 
                break;
        }
    }
    return "";
}

bool Instruction::isInRange(int value, int minRange, int maxRange) {
    return (value >= minRange && value <= maxRange);
}

void Instruction::handle_inst(int argc, char* argv[])
{
    string cValue = getParameterValue(argc, argv, "-c");
    string nValue = getParameterValue(argc, argv, "-n");
    string mValue = getParameterValue(argc, argv, "-m");
    string rValue = getParameterValue(argc, argv, "-r");

    if (strcmp(argv[1], "-c") == 0) {
        cout << "cvalue"<<cValue << endl;
        if (!cValue.empty()) {
            c = stoi(cValue);
            if (isInRange(c, 1, 1000000)) {
                cout << "生成" << c << "个数独终盘" << endl;
                return;
            }
            else {
                cout << "终局数量超出范围，请提供1-1000000之间的值" << endl;
                return;
            }
        }
        else {
            cout << "参数缺失，请按照 shudu.exe -c <count> 的格式输入参数" << std::endl;
        }
    }
    else {
        if (!nValue.empty()) {
            n = stoi(nValue);//解析游戏数量
            if (!isInRange(n, 1, 10000))
            {
                cout << "错误：游戏数量范围应为1-10000" << endl;
                return;
            }
            if (!mValue.empty())//如果提供了-m参数
            {
                m = stoi(mValue);//解析游戏难度
                if (!isInRange(m, 1, 3)) {
                    cout << "游戏难度超出范围，请选择 1、2 或 3" << endl;
                    return;
                }
            }
            if (!rValue.empty()) {//如果提供了-r参数
                // 解析挖空数量范围
                size_t rangePos = rValue.find("~");
                if (rangePos == string::npos) {
                    cout << "参数错误，请按照 shudu.exe -n <count> -r <minHoles~maxHoles> 的格式输入参数" << endl;
                    return;
                }
                string rMinValue = rValue.substr(0, rangePos);
                string rMaxValue = rValue.substr(rangePos + 1);
                rMin = stoi(rMinValue);
                rMax = stoi(rMaxValue);
                if (!isInRange(rMin, 20, 55) || !isInRange(rMax, 20, 55) || rMin > rMax) {
                    cout << "挖空数量范围无效，请提供20-55之间的有效范围" << endl;
                    return;
                }
            }
            for (int i = 1; i < argc; i++) {
                string arg = argv[i];
                if (arg == "-u")
                {
                    uFlag = true;
                }
            }
        }
        else {
            cout << "参数错误，请按照 shudu.exe -n <count> -m <mode> -r <minHoles~maxHoles>的格式输入参数" << endl;
        }
    }
    if (c != 0) {
        cout << "生成" << c << "个数独终局" << endl;
    }
    if (n != 0) {
        cout << "生成" << n << "个游戏";
    }
    if (m != 0) {
        cout << "难度为:" << m << endl;
    }
    if (rMin != 0 && rMax != 0)
    {
        cout << "，挖空数量范围：" << rMin << "~" << rMax;
    }
    if (uFlag)
    {
        cout << "，解唯一";
    }
    return;
}

int main(int argc, char* argv[])
{
    Instruction inst;
    //ShuduGame shuduGame;
    inst.handle_inst( argc, argv);
    return 0;
}