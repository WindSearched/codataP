//
// Created by ws on 2026/10/12.
//

#ifndef CODATAP_BRANCH_H
#define CODATAP_BRANCH_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class BranchArgument {
public:
    std::string arg;
    std::function<std::string()> suggestion;

    BranchArgument(std::string arg);

    BranchArgument& SetSuggestion(std::function<std::string()> suggestion);
};

class CommandArgument {
public:
    std::unordered_map<std::string, std::string> args;

    std::string Get(std::string key);
    bool TryGet(std::string key, std::string& value);

    std::string ParamGet(int index);
    bool TryParamGet(int index, std::string& value);

    void Set(std::string key, std::string value);
};

class Branch {
private:
    std::function<std::string(CommandArgument)> execute;

public:
    std::string name;
    std::function<std::vector<std::string>(BranchArgument)> suggestion;
    std::vector<Branch> branches;
    std::vector<BranchArgument> args;
    std::string description;
    bool param;
    std::string abbreviation;

    Branch(std::string name);

    Branch& AddBranch(Branch branch);
    Branch& AddArgument(BranchArgument argument);
    Branch& AddExecution(std::function<std::string(CommandArgument)> execution);

    Branch* Parse(std::vector<std::string> split, CommandArgument& args);

    std::string Run(CommandArgument& args);
};

#endif // CODATAP_BRANCH_H