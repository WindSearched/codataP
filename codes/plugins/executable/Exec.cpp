//
// Created by ws on 2026/11/2.
//

#include "Exec.h"

#include "../../Core.h"
#include "../../Data.h"

using string = std::string;

void Exec::SetExecute(Branch *branch, string cmd, bool alsoBranches = true) {
    string command = cmd + " " + branch->name;
    branch->execute = new CdExec(command);

    if (alsoBranches && !branch->branches.empty()) {
        for (auto& branch : branch->branches) {
            SetExecute(&branch, command, true);
        }
    }
}

void Exec::SetExecute(std::vector<Branch> *branches, std::string cmd, bool alsoBranches) {
    for (auto& branch : *branches) {
        SetExecute(&branch, cmd, alsoBranches);
    }
}

bool Exec::Load(std::filesystem::path path) {
    if (!Data::IsExecutable(path)) return false;

    auto t = Data::ExecuteCommand(path.string() + " codata");
    if (t.back() == '\n')
        t.pop_back();
    if (t != "codata") return false;

    std::string startPut = Data::ExecuteCommand(path.string() + " start");
    Core::mainWindow->LogLine(startPut);

    string branchesOut = Data::ExecuteCommand(path.string() + " branches");
    auto bs = Branch::FromString(branchesOut);
    SetExecute(&bs, path.string() + " exec", true);
    Core::branch.AddBranches(bs);



    return true;
}

bool Exec::Loads(std::filesystem::path pluginPath) {
    if (!Data::IsDirectory(pluginPath)) {
        Data::CreateDirectory(pluginPath);
    }
    for (const auto& file : std::filesystem::directory_iterator(pluginPath)) {
        if (file.is_regular_file()) {
            if (Data::IsExecutable(file)) {
                Exec::Load(file.path().string());
            }
        }
    }
    return true;
}
