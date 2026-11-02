
#include "Branch.h"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <string>
#include <variant>

#include "Core.h"
#include "Sets.h"
#include "classes/CommandArgument.h"

BranchArgument::BranchArgument(std::string arg)
    : arg(std::move(arg)) {
}

BranchArgument& BranchArgument::SetSuggestion(
    std::function<std::vector<std::string>(std::string)> suggestion) {
    this->suggestion = std::move(suggestion);
    return *this;
}

std::vector<std::string> BranchArgument::GetSuggestion(
    std::string put) const {
    if (!suggestion) {
        return {};
    }

    return suggestion(put);
}

BranchArgument BranchArgument::boolean(std::string name) {
    BranchArgument argument(std::move(name));

    argument.SetSuggestion(
        [](std::string) {
            return std::vector<std::string>{"true", "false"};
        }
    );

    return argument;
}

// --------------------------------------------------
// Branch
// --------------------------------------------------

Branch::Branch(std::string name)
    : name(std::move(name)),
      param(false) {
    execute = nullptr;
}

Branch& Branch::AddBranch(Branch branch) {
    branches.push_back(std::move(branch));
    return *this;
}

Branch & Branch::AddBranches(std::vector<Branch> branches) {
    if (branches.empty()) return *this;
    for (const auto & branch : branches) {
        this->branches.push_back(branch);
    }
    return *this;
}

Branch* Branch::AddBranchReturnSelf(Branch branch) {
    branches.push_back(std::move(branch));
    return &branches.back();
}

Branch& Branch::AddBranch(std::string name) {
    return AddBranch(Branch(std::move(name)));
}

Branch* Branch::AddBranchReturnSelf(std::string name) {
    return AddBranchReturnSelf(Branch(std::move(name)));
}

Branch& Branch::AddArgument(BranchArgument argument) {
    args.push_back(std::move(argument));
    return *this;
}

Branch& Branch::AddArgument(std::string name) {
    return AddArgument(BranchArgument(std::move(name)));
}

Branch& Branch::AddExecution(std::function<std::string(CommandArgument)> execution) {
    delete execute;
    execute = new CdLambda(execution);
    return *this;
}

Branch& Branch::SetParam() {
    param = true;
    return *this;
}

// --------------------------------------------------
// Parse
// --------------------------------------------------

Branch* Branch::Parse(
    std::vector<std::string> split,
    CommandArgument& commandArgs) {

    if (split.empty()) {
        return this;
    }

    // 1. 优先匹配子 Branch
    const std::string& head = split.front();

    if (!head.empty()) {
        for (auto& branch : branches) {
            if (branch.name == head ||
                (!branch.abbreviation.empty() &&
                 branch.abbreviation == head)) {

                split.erase(split.begin());
                return branch.Parse(std::move(split), commandArgs);
            }
        }
    }

    // 2. 当前节点支持自由参数
    if (param) {
        for (std::size_t i = 0; i < split.size(); ++i) {
            commandArgs.Set(
                "param" + std::to_string(i + 1),
                split[i]
            );
        }

        return this;
    }

    // 3. 尝试匹配当前节点声明的参数
    // 最后一个空字符串视为命令行末尾的占位符
    std::size_t tokenCount = split.size();

    if (!split.empty() && split.back().empty()) {
        --tokenCount;
    }

    if (tokenCount <= args.size()) {
        for (std::size_t i = 0; i < tokenCount; ++i) {
            commandArgs.Set(args[i].arg, split[i]);
        }
    }

    // 4. 无法继续匹配时返回当前节点
    return this;
}

// --------------------------------------------------
// Run
// --------------------------------------------------

std::string Branch::Run(CommandArgument& commandArgs) {
    return execute->Run(commandArgs);
}

std::string Branch::Command(std::vector<std::string> split) {
    CommandArgument commandArgs;

    Branch* node = Parse(std::move(split), commandArgs);

    if (!node) {
        return "Cannot find command branch";
    }

    return node->Run(commandArgs);
}

std::string Branch::ToString(Branch &branch) {
    std::string s = "";

    s += branch.name;

    if (branch.branches.size() > 0) {
        s += '{';
        for (auto& b : branch.branches) {
            std::string t = ToString(b);
            s += t;
            s += ',';
        }
        s.pop_back();//remove " , "
        s += '}';
    }
    if (branch.args.size() > 0) {
        s += '[';
        for (auto& b : branch.args) {
            s+= b.arg;
            s += ',';
        }
        s.pop_back(); // remove ' , '
        s += ']';
    }

    return s;
}

std::string Branch::ToString(std::vector<Branch> branches) {
    string s = "";
    for (auto & branch : branches) {
        s += ToString(branch);
        s += ',';
    }
    s.pop_back();
    return s;
}

std::vector<Branch> Branch::FromString(std::string text) {
    std::vector<Branch> branches;

    size_t init = std::variant_npos, final = std::variant_npos;
    char initc = ' ';
    for (int i = 0; i < text.length(); i++) {
        char c = text[i];
        if (c == '{' || c == '[' || c == ',') {
            init = 0, final = i;
            initc = c;
            break;
        }
    }
    if (init == std::variant_npos) {
        branches.push_back(Branch(text));
        return branches;
    }

    auto b = Branch(text.substr(init, final - init ));

    if (initc == '{') {
        init = final;
        int c = 0;
        for (size_t i = init; i < text.length(); i++) {
            char ch = text[i];
            if (ch == '{')
                c ++;
            else if (ch == '}') {
                if (--c == 0) {
                    final = i + 1;
                    break;
                }
            }
        }

        if (final != init) {
            std::string child = text.substr(init + 1, final - init - 2);
            auto cb = FromString(child);
            b.AddBranches(cb);
        }
        else
            return {};

        initc = text[final];
    }
    // if text finished
    if (final + 1 >= text.length()) {
        branches.push_back(b);
        return branches;
    }

    if (initc == '[') {
        int lastInit = final +1;
        for (size_t i = final + 1; i < text.length(); i++) {
            if (text[i] == ',') {
                b.AddArgument(text.substr(lastInit, i + 1 - lastInit));
                lastInit = i + 1;
            }
            else if (text[i + 1] == ']') {
                b.AddArgument(text.substr(lastInit, i +1 - lastInit));
                lastInit = i + 1;
                break;
            }
        }
        init = final;
        final = lastInit + 1;

        initc = text[final];
    }

    // if the text finished
    if (final +1 >= text.length()) {
        branches.push_back(b);
        return branches;
    }

    if (initc == ',') {
        std::string others = text.substr(final + 1);
        branches = FromString(others);
    }

    branches.push_back(b);

    return branches;
}
