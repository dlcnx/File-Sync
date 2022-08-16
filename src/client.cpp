#include "client.h"

// 初始化
ModClient::ModClient(const json &json_obj) {
    this->info_dirs = json_obj["dirs"];
    this->info_types = json_obj["file_type"];
    this->info_basics = json_obj["basic"];
    this->createFileList();
    this->initUpdateFiles();
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
                this->local_files.emplace_back(file_path, file_hash);
            }
        }
    }
}

// 生成同步列表
void ModClient::initUpdateFiles() {
    // 转换成相同类型
    vector<pair<string, string>> temp_basics;
    for (const auto &server_file: this->info_basics) {
        temp_basics.emplace_back(server_file[0], server_file[1]);
    }
    // 分别取差集
    this->download_files = ModClient::getDifferentFiles(temp_basics, this->local_files);
    this->remove_files = ModClient::getDifferentFiles(this->local_files, temp_basics);
}

// 判断字符串是否在json列表中
bool ModClient::isExist(const string &str, const json &list) {
    if (any_of(list.begin(), list.end(), [&str](const auto &ext) { return ext == str; })) {
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
    int pos = (int) result.find('\n');
    return result.substr(pos + 1, 41);
}

// 取差集
vector<string> ModClient::getDifferentFiles(const vector<pair<string, string>> &first_files,
                                            const vector<pair<string, string>> &second_files) {
    vector<string> temp_files;
    for (const auto &file: first_files) {
        if (none_of(second_files.begin(), second_files.end(),
                    [file](const auto &ext) { return file.second == ext.second; })) {
            temp_files.push_back(file.first);
        }
    }
    return temp_files;
}