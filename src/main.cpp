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
    if (content.empty()) {
        return;
    }
    ModClient client(content);
}

int main() {
    //判断系统并切换编码
#if defined(WINNT) || defined(_WIN32) || defined(_WIN64)
    system("chcp 65001");
#endif
    test1();
    return 0;
}
