#include "client.h"

// 解析字符串并初始化
ModClient::ModClient(const string &content) {
    Json::Reader reader;
    auto *root = new Json::Value;
    reader.parse(content, *root);
    this->info_dirs = (*root)["dirs"];
    this->info_types = (*root)["file_type"];
    this->info_basics = (*root)["basic"];
    delete root;
    createFileList();
}

// 遍历目录初始化本地需同步的文件列表
void ModClient::createFileList() {
    for (const auto &info_dir: this->info_dirs) {
        string dir_name = info_dir.asString();
        // 文件夹不存在则创建
        if (!filesystem::is_directory(dir_name)) {
            filesystem::create_directories(dir_name);
        }
        // 将符合后缀名的文件添至列表
        for (const auto &file: filesystem::directory_iterator(dir_name)) {
            // 是否为文件
            if (file.is_regular_file()) {
                // 文件类型是否在同步类型中
                if (isExist(file.path().extension().string(), this->info_types)) {
                    string file_path = regex_replace(file.path().string(), regex("\\\\"), "/");
                    string file_hash = "1234567890";
                    this->local_files.push_back(vector<string>{file_path, file_hash});
                }
            }
        }
    }
}

bool ModClient::isExist(const string &str, const Json::Value &list) {
    for (const auto &ext: list) {
        if (ext.asString() == str) {
            return true;
        }
    }
    return false;
}
