目录说明：

test.txt：对debug模式下生成的可执行文件进行测试。运行时将后缀.txt改为.bat即可运行产生每一个测试用例的代码覆盖率（以二进制的形式保存.cov）

test_binary：存放每个测试用例的二进制代码覆盖率

integration_run.py：将二进制代码的代码覆盖率合并得到集成测试的结果并输出到html文件中。

coverage.html：集成测试的结果，可在index.html中查看具体的代码覆盖率。