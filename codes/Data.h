//
// Created by ws on 2026/10/26.
//

#ifndef CODATAP_DATA_H
#define CODATAP_DATA_H
#include <filesystem>
#include <yaml-cpp/yaml.h>

#include "Branch.h"


class Data {
public:
    static Branch branch;
    static Branch setsBranch;
    static std::filesystem::path currentPath;
    static std::filesystem::path setsPath;
    static std::filesystem::path pluginsPath;

    static std::filesystem::path ToPath(std::string path);
    static bool IsFile(std::filesystem::path path);
    static bool IsDirectory(std::filesystem::path path);
    static std::filesystem::path GetProximatePath(std::filesystem::path path);
    static bool IsExecutable(std::filesystem::path);
    static bool IsExtension(std::filesystem::path path ,std::string extension);
    static bool IsExtension(std::string path ,std::string extension);

    static std::string Read(std::filesystem::path path);
    static bool Write(std::filesystem::path path, std::string data);
    static void CreateDirectory(std::filesystem::path path);
    static bool OpenTextDefault(std::filesystem::path);
    static std::string ExecuteCommand(std::string command);

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static std::string HttpGet(const std::string& url);
};


#endif //CODATAP_DATA_H
