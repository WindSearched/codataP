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

    static std::filesystem::path ToPath(const std::string &path);
    static bool IsFile(const std::filesystem::path &path);
    static bool IsDirectory(const std::filesystem::path& path);
    static std::filesystem::path GetProximatePath(const std::filesystem::path &path);
    static bool IsExecutable(const std::filesystem::path&);
    static bool IsExtension(const std::filesystem::path& path ,const std::string& extension);
    static bool IsExtension(const std::string& path ,const std::string& extension);

    static std::string Read(const std::filesystem::path& path);
    static bool Write(const std::filesystem::path& path, const std::string& data);
    static void CreateDirectory(const std::filesystem::path& path);
    static bool OpenTextDefault(const std::filesystem::path&);
    static std::string ExecuteCommand(const std::string& command);

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static std::string HttpGet(const std::string& url);
};


#endif //CODATAP_DATA_H
