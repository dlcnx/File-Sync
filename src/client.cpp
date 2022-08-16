#include "client.h"

// 解析字符串并初始化
ModClient::ModClient(const json &json_obj) {
    this->info_dirs = json_obj["dirs"];
    this->info_types = json_obj["file_type"];
    this->info_basics = json_obj["basic"];
    createFileList();
}

// 遍历目录初始化本地需同步的文件列表
void ModClient::createFileList() {
    for (const auto &info_dir: this->info_dirs) {
        string dir_name = info_dir;
        // 文件夹不存在则创建
        if (!filesystem::is_directory(dir_name)) {
            filesystem::create_directories(dir_name);
        }
        // 将符合后缀名的文件添至列表
        for (const auto &file: filesystem::directory_iterator(dir_name)) {
            // 是否为文件
            if (!file.is_regular_file()) {
                continue;
            }
            // 类型是否在同步类型中
            if (isExist(file.path().extension().string(), this->info_types)) {
                string file_path = regex_replace(file.path().string(), regex("\\\\"), "/");
                string file_hash = ModClient::getFileHash(file_path);
                this->local_files.push_back(vector<string>{file_path, file_hash});
            }
        }
    }
}

// 判断字符串是否在json列表中
bool ModClient::isExist(const string &str, const json &list) {
    if (any_of(list.begin(), list.end(), [&str](const auto &ext){return ext == str;})) {
        return true;
    }
    return false;
}

// 调用系统命令获取哈希值
string ModClient::getFileHash(const string &file_path) {
    std::string cmd = "certutil -hashfile " + file_path;
    std::array<char, 128> buffer{};
    std::string result;
    // 执行命令获取结果
    std::unique_ptr<FILE> pipe(popen(cmd.c_str(), "r"));
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    int pos = (int)result.find('\n');
    return result.substr(pos + 1, 41);
}
