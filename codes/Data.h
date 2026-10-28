//
// Created by ws on 2026/10/26.
//

#ifndef CODATAP_DATA_H
#define CODATAP_DATA_H
#include <filesystem>
#include <yaml-cpp/yaml.h>

#include "../CdPlugin/codes/Branch.h"


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

    static std::string Read(std::filesystem::path path);
    static bool Write(std::filesystem::path path, std::string data);
    static void CreateDirectory(std::filesystem::path path);
    static bool OpenTextDefault(std::filesystem::path);
};


#endif //CODATAP_DATA_H
