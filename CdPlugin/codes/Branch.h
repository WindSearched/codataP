//
// Created by ws on 2026/10/12.
//

#ifndef CODATAP_BRANCH_H
#define CODATAP_BRANCH_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../codes/Sets.h"

class BranchArgument {
public:
    std::string arg;
    std::function<std::vector<std::string>(std::string currentArg)> suggestion;

    BranchArgument(std::string arg);

    BranchArgument& SetSuggestion(std::function<std::vector<string>(string)> suggestion);
    std::vector<std::string> GetSuggestion(std::string put);

    static BranchArgument boolean(string name);
private:

};

class CommandArgument {
public:
    std::unordered_map<std::string, std::string> args;

    std::string Get(std::string key);
    bool TryGet(std::string key, std::string& value);

    int GetInt(std::string key);
    bool TryGetInt(std::string key, int& value);

    bool GetBool(std::string key);
    bool TryGetBool(std::string key, bool& value);

    std::string ParamGet(int index);
    bool TryParamGet(int index, std::string& value);

    std::vector<std::string> GetList();

    void Set(std::string key, std::string value);
    std::vector<std::string> GetArgs() {
        std::vector<std::string> o;
        for (const auto& arg : args) {
            o.push_back(arg.second);
        }
        return o;
    }
    int Count() {
        return args.size();
    }
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
    Branch& AddBranch(std::string name);
    Branch& AddArgument(BranchArgument argument);
    Branch& AddArgument(std::string name);
    Branch& AddExecution(std::function<std::string(CommandArgument)> execution);
    Branch& SetParam();

    Branch* Parse(std::vector<std::string> split, CommandArgument& args);

    std::string Run(CommandArgument& args);
    std::string Command(std::vector<std::string> split);
};

#endif // CODATAP_BRANCH_H