#include "client.h"

namespace dlcn {
    // 初始化
    ClientController::ClientController(const json &json_obj) {
        this->info_dirs = json_obj["dirs"];
        this->info_types = json_obj["file_type"];
        this->info_basics = json_obj["basic"];
        this->createFileList();
        this->initUpdateFiles();
    }

    // 遍历目录初始化本地需同步的文件列表
    void ClientController::createFileList() {
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
                    string file_hash = ClientController::getFileHash(file_path);
                    this->local_files.emplace_back(file_path, file_hash);
                }
            }
        }
    }

    // 生成同步列表
    void ClientController::initUpdateFiles() {
        // 转换成相同类型
        vector<pair<string, string>> temp_basics;
        for (const auto &server_file: this->info_basics) {
            temp_basics.emplace_back(server_file[0], server_file[1]);
        }
        // 分别取差集
        this->download_files = ClientController::getDifferentFiles(temp_basics, this->local_files);
        this->remove_files = ClientController::getDifferentFiles(this->local_files, temp_basics);
    }

    // 判断字符串是否在json列表中
    bool ClientController::isExist(const string &str, const json &list) {
        if (any_of(list.begin(), list.end(), [&str](const auto &ext) { return ext == str; })) {
            return true;
        }
        return false;
    }

    // 调用系统命令获取哈希值
    string ClientController::getFileHash(const string &file_path) {
        // 不同平台的命令
        string cmd;
#if defined(WINNT) || defined(_WIN32) || defined(_WIN64)
        cmd = "certutil -hashfile " + file_path + " MD5";
#else
        cmd = "md5sum " + file_path;
#endif
        array<char, 128> buffer{};
        string result;
        // 执行命令获取结果
        unique_ptr<FILE> pipe(popen(cmd.c_str(), "r"));
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        // 不同平台的起始位置
#if defined(WINNT) || defined(_WIN32) || defined(_WIN64)
        int pos = (int) result.find('\n') + 1;
#else
        int pos = 0;
#endif
        return result.substr(pos, 32);
    }

    // 取差集
    vector<string> ClientController::getDifferentFiles(const vector<pair<string, string>> &first_files,
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
}