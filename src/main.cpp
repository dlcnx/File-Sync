#include <fstream>
#include "handler.h"
#include "sion.h"
using namespace dlcn;

void test1() {
    ifstream json_file("test.json");
    json content = json::parse(json_file);
    ClientController client(content);
    cout << ClientController::getFileHash("test.json");
}

void test2() {
    string url = "https://baidu.com";
    auto resp = sion::Fetch(url);
    std::cout << resp.StrBody() << std::endl;
    std::cout << resp.GetHeader().Get("content-type") << std::endl;
}

int main() {
    //判断系统并切换编码
#if defined(WINNT) || defined(_WIN32) || defined(_WIN64)
    system("chcp 65001");
#endif
    // test1();
    test2();
    return 0;
}
