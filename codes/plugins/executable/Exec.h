//
// Created by ws on 2026/11/2.
//

#ifndef CODATAP_EXEC_H
#define CODATAP_EXEC_H
#include <filesystem>

#include "../../Branch.h"


class Exec {
public:

    static void SetExecute(Branch* branch, std::string cmd, bool alsoBranches);
    static void SetExecute(std::vector<Branch> * branches, std::string cmd, bool alsoBranches);
    static bool Load(std::filesystem::path);
    static bool Loads(std::filesystem::path pluginPath);
};


#endif //CODATAP_EXEC_H
