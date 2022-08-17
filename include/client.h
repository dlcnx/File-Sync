#pragma once

#include <iostream>
#include <vector>
#include <regex>
#include <filesystem>
#include "json.hpp"

namespace dlcn {
    using json = nlohmann::json;
    using namespace std;

    class ClientController {
    private:
        json info_dirs;
        json info_types;
        json info_basics;
        vector<pair<string, string>> local_files;
    public:
        vector<string> download_files;
        vector<string> remove_files;

        explicit ClientController(const json &json_obj);

        void createFileList();

        void initUpdateFiles();

        static bool isExist(const string &str, const json &list);

        static string getFileHash(const string &file_path);

        static vector<string> getDifferentFiles(const vector<pair<string, string>> &first_files,
                                                const vector<pair<string, string>> &second_files);
    };
}
