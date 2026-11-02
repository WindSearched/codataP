
#ifndef CODATAP_BRANCH_H
#define CODATAP_BRANCH_H

#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "classes/CdFunc.h"
#include "classes/CommandArgument.h"

class BranchArgument {
public:
    std::string arg;

    std::function<
        std::vector<std::string>(std::string currentArg)
    > suggestion;

    explicit BranchArgument(std::string arg);

    BranchArgument& SetSuggestion(
        std::function<std::vector<std::string>(std::string)> suggestion
    );

    std::vector<std::string> GetSuggestion(std::string put) const;

    static BranchArgument boolean(std::string name);
};

class Branch {
private:
    CdFunc* execute;

public:
    std::string name;

    std::function<
        std::vector<std::string>(BranchArgument)
    > suggestion;

    std::vector<Branch> branches;
    std::vector<BranchArgument> args;

    std::string description;
    bool param;
    std::string abbreviation;

    explicit Branch(std::string name);

    Branch& AddBranch(Branch branch);
    Branch& AddBranches(std::vector<Branch> branches);
    Branch* AddBranchReturnSelf(Branch branch);

    Branch& AddBranch(std::string name);
    Branch* AddBranchReturnSelf(std::string name);

    Branch& AddArgument(BranchArgument argument);
    Branch& AddArgument(std::string name);

    Branch& AddExecution(
        std::function<std::string(CommandArgument)> execution
    );

    Branch& SetParam();

    Branch* Parse(
        std::vector<std::string> split,
        CommandArgument& args
    );

    std::string Run(CommandArgument& args);

    std::string Command(std::vector<std::string> split);

    static std::string ToString(Branch& branch);
    static std::string ToString(std::vector<Branch> branches);
    // insert in branch
    static std::vector<Branch> FromString(std::string text);
};

#endif // CODATAP_BRANCH_H
