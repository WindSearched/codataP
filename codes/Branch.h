//
// Created by ws on 2026/10/12.
//

#ifndef CODATAP_BRANCH_H
#define CODATAP_BRANCH_H
#include <functional>
#include <ranges>
#include <string>

class BranchArgument{
public:
    std::string arg;
    std::function<std::string()> suggestion;

    BranchArgument(std::string arg): arg(arg) {}
    BranchArgument& SetSuggestion(std::function<std::string()> suggestion) {
        this->suggestion = suggestion;
        return *this;
    }
};
class CommandArgument {
public:
    std::unordered_map<std::string, std::string> args;

    std::string Get(std::string key) {
        return args.find(key)->second;
    }
    bool TryGet(std::string key, std::string& value) {
        auto i = args.find(key);
        if (i != args.end()) {
            value = i->second;
            return true;
        }
            return false;
    }
    std::string ParamGet(int index) {
        return  Get("param" + std::to_string(index));
    }
    bool TryParamGet(int index, std::string& value) {
        return TryGet("param" + std::to_string(index), value);
    }

    void Set(std::string key, std::string value) {
        args[key] = value;
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

    Branch(std::string name) : name(name) {}

    Branch& AddBranch(Branch branch) {
        branches.push_back(branch);
        return *this;
    }
    Branch& AddArgument(BranchArgument& argument) {
        args.push_back(argument);
        return *this;
    }
    Branch& AddExecution(std::function<std::string(CommandArgument)> execution) {
        execute = execution;
        return *this;
    }


    Branch& Parse(std::vector<std::string> split, CommandArgument& args) {

        if (split.size() == 0) return *this;
        auto head = split[0];
        auto adaptedBranch = branches
            | std::views::filter([head](Branch x)
                { return x.name == head || x.abbreviation == head;});
        std::vector<Branch> ab(adaptedBranch.begin(), adaptedBranch.end());


        if (param && ab.size() == 0)
        {
            for (int i = 0; i < split.size(); i++) {
                args.Set("param"+ std::to_string(i), split[i]);
            }
            return *this;
        }
        else {
            for (auto b : ab) {
                split.erase(split.begin());
                return b.Parse(split, args);
            }
        }

        if (split.size() != this->args.size()) {
            return *this;
        }

        for (int i = 0; i < this->args.size(); i++) {
            args.Set(this->args[i].arg, split[i]);
        }
        return *this;
    }

    std::string Run(CommandArgument& args) {
        return execute(args);
    }
};



#endif //CODATAP_BRANCH_H
