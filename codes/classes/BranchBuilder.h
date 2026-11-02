//
// Created by ws on 2026/10/31.
//

#ifndef CODATAP_BRANCHBUILDER_H
#define CODATAP_BRANCHBUILDER_H
#include <stack>

#include "../Branch.h"


class BranchBuilder {
public:
    Branch branch;
    std::stack<Branch*> stack;

    BranchBuilder();

    bool MovePointer(std::string to);
    bool MovePointer(int index);

    void InsertBranch(std::string branch, bool movePointer = true);
    bool Back();

    void AddArgument(std::string arg);
    void AddExecute(std::function<std::string(CommandArgument)> func);

    Branch Build();
    void Clear();
};


#endif //CODATAP_BRANCHBUILDER_H
