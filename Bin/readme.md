
sudu_release ：visual studio2019 解决方案配置为release x86

sudu_debug ：visual studio2019 解决方案配置为debug x86

shudu.exe：gcc version 5.1.0 (tdm64-1)
	命令行输入：`g++ -std=c++11 shudu.cpp -o shudu.exe`



| 参数名称 | 参数意义                   | 范围             | 用法示例                   |
| -------- | -------------------------- | ---------------- | -------------------------- |
| -c       | 需要的数独终盘数量         | 1-1000000        | shudu.exe -c 2             |
| -s       | 需要解的数独棋盘文件路径   | 绝对或者相对路径 | shudu.exe -s game.txt      |
| -n       | 需要的游戏数量             | 1-10000          | shudu.exe -n 1000          |
| -m       | 生成游戏的难度             | 1-3              | shudu.exe -n 100 -m 1      |
| -r       | 生成游戏中的挖空的数量范围 | 20-55            | shudu.exe -n 10 -r 20~55   |
| -u       | 生成游戏的解唯一           |                  | shudu.exe -n 10 -u         |
| -N      | 多解情况下最多输出的解个数 |                  | shudu.exe -s game.txt -N 2 |

注意：m、r、u只有和n一起使用才认为参数无误，否则报错。

N只有和s在一起才有效。