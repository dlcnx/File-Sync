#pragma once
#include <iostream>
#include <vector>
#include <regex>
#include <filesystem>
#include "json/json.h"
using namespace std;

class ModClient {
private:
    Json::Value info_dirs;
    Json::Value info_types;
    Json::Value info_basics;
    vector<vector<string>> local_files;
public:
    vector<vector<string>> remove_files;
    vector<vector<string>> download_files;
    explicit ModClient(const string &content);
    void createFileList();
    static bool isExist(const string &str, const Json::Value &list);
};
