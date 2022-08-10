#include <iostream>
#include <fstream>
#include "client.h"

using namespace std;

// 读取文件返回字符串
string readFile(const string &file_name) {
    ifstream fin(file_name, ios::in);
    stringstream buf;
    buf << fin.rdbuf();
    fin.close();
    return buf.str();
}

void test1() {
    string content = readFile("test.json");
    ModClient client(content);
}

int main() {
    // 切换系统编码
    system("chcp 65001");
    test1();
    return 0;
}
