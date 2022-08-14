#pragma once
#include <iostream>
#include <vector>
#include <regex>
#include <filesystem>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

class ModClient {
private:
    json info_dirs;
    json info_types;
    json info_basics;
    vector<vector<string>> local_files;
public:
    vector<vector<string>> remove_files;
    vector<vector<string>> download_files;
    explicit ModClient(const string &content);
    void createFileList();
    static bool isExist(const string &str, const json &list);
};
