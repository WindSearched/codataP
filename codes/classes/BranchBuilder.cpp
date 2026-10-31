//
// Created by ws on 2026/10/31.
//

#include "BranchBuilder.h"

#include <utility>

BranchBuilder::BranchBuilder() : branch("builder") {
}

bool BranchBuilder::MovePointer(string to) {
    if (to == "..") {
        if (stack.empty()) return false;
        stack.pop();
        return true;
    }

    auto* p = stack.top();
    for (auto& n : p->branches) {
        if (n.name == to) {
            stack.push(&n);
            return true;
        }
    }
    return false;
}

bool BranchBuilder::MovePointer(int index) {
    if (index == -1) {
        if (stack.empty()) return false;
        stack.pop();
        return true;
    }

    auto* p = stack.top();
    if (index >= p->branches.size()) return false;
    auto& b = p->branches[index];
    stack.push(&b);

    return true;
}

void BranchBuilder::InsertBranch(string branchName, bool movePointer) {
    if (stack.empty()) {
        branch = Branch(branchName) ;
        stack.push(&branch);
        return;
    }
    auto* p = stack.top();
    auto* child = p->AddBranchReturnSelf(std::move(branchName));
    if (movePointer)
        stack.push(child);
}

bool BranchBuilder::Back() {
    if (stack.empty()) return false;
    stack.pop();
    return true;
}

void BranchBuilder::AddArgument(string arg) {
    auto* p = stack.top();
    p->AddArgument(arg);
}

void BranchBuilder::AddExecute(std::function<string(CommandArgument)> func) {
    auto* p = stack.top();
    p->AddExecution(func);
}

Branch BranchBuilder::Build() {
    return branch;
}

void BranchBuilder::Clear() {
    stack = std::stack<Branch*>();
}
