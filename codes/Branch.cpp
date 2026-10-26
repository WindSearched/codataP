//
// Created by ws on 2026/10/12.
//

#include "Branch.h"

#include <limits>
#include <stdexcept>

using string = std::string;

BranchArgument::BranchArgument(std::string arg)
    : arg(arg) {
}

BranchArgument& BranchArgument::SetSuggestion(
    std::function<std::string()> suggestion) {
    this->suggestion = suggestion;
    return *this;
}

std::string CommandArgument::Get(std::string key) {
    return args.find(key)->second;
}

bool CommandArgument::TryGet(std::string key, std::string& value) {
    auto i = args.find(key);

    if (i != args.end()) {
        value = i->second;
        return true;
    }

    return false;
}

int CommandArgument::GetInt(std::string key) {
    auto i = Get(key);
    return std::stoi(i);
}

bool CommandArgument::TryGetInt(std::string key, int &value) {
    std::string s;
    if (TryGet(key, s)) {
        try {
            value = std::stoi(s);
            return  true;  // 抛 invalid_argument
        } catch (const std::invalid_argument&) {
            // 不是数字
            value = std::numeric_limits<int>::min();
            return false;
        } catch (const std::out_of_range&) {
            // 数字太大/太小
            value = std::numeric_limits<int>::max();
            return false;
        }
    }
    return false;
}

bool CommandArgument::GetBool(std::string key) {
    string s = Get(key);
    return s == "true" || s == "1" || s == "y";
}

bool CommandArgument::TryGetBool(std::string key, bool &value) {
    string s = Get(key);
    if (s == "true" || s == "1" || s == "y") {
        value = true;
        return true;
    }
    else if (s == "false" || s == "0" || s == "n") {
        value = false;
        return true;
    }
    return false;
}

std::string CommandArgument::ParamGet(int index) {
    return Get("param" + std::to_string(index));
}

bool CommandArgument::TryParamGet(int index, std::string& value) {
    return TryGet("param" + std::to_string(index), value);
}

void CommandArgument::Set(std::string key, std::string value) {
    args[key] = value;
}

Branch::Branch(std::string name)
    : name(name), param(false) {
}

Branch& Branch::AddBranch(Branch branch) {
    branches.push_back(branch);
    return *this;
}

Branch & Branch::AddBranch(std::string name) {
    return AddBranch(Branch(name));
}

Branch& Branch::AddArgument(BranchArgument argument) {
    args.push_back(argument);
    return *this;
}

Branch & Branch::AddArgument(std::string name) {
    return AddArgument(BranchArgument(name));
}

Branch& Branch::AddExecution(
    std::function<std::string(CommandArgument)> execution) {
    execute = execution;
    return *this;
}

Branch* Branch::Parse(
    std::vector<std::string> split,
    CommandArgument& args) {

    if (split.empty()) {
        return this;
    }

    auto head = split[0];

    Branch* matched = nullptr;

    if (!head.empty())
        for (auto& b : branches) {
            if (b.name == head || b.abbreviation == head) {
                matched = &b;
                break;
            }
        }

    if (this->param && matched == nullptr) {
        for (size_t i = 0; i < split.size(); i++) {
            args.Set("param" + std::to_string(i), split[i]);
        }

        return this;
    }
    else if (matched != nullptr) {
        split.erase(split.begin());
        return matched->Parse(split, args);
    }

    if (split.size() <= this->args.size())
        for (size_t i = 0; i < split.size(); i++) {
            args.Set(this->args[i].arg, split[i]);
        }

    return this;
}

std::string Branch::Run(CommandArgument& args) {
    if (execute) {
        return execute(args);
    }

    return "Cannot found this command branch ";
}

std::string Branch::Command(std::vector<std::string> split) {
    CommandArgument args;
    auto* node = Parse(split, args);
    return node->Run(args);
}
