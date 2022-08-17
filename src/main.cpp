#include <fstream>
#include "handler.h"

using namespace dlcn;

void test1() {
    ifstream json_file("test.json");
    json content = json::parse(json_file);
    ClientController client(content);
    cout << ClientController::getFileHash("test.json");
}

int main() {
    //判断系统并切换编码
#if defined(WINNT) || defined(_WIN32) || defined(_WIN64)
    system("chcp 65001");
#endif
    test1();
    return 0;
}
